#include "MumbleLink.hpp"

#include <components/esm/loadcell.hpp>

#include <algorithm>
#include <cstring>
#include <ctime>

#include <fmt/format.h>

#ifndef _WIN32
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#endif

namespace mwmp
{
    namespace
    {
        std::string getTime()
        {
            time_t t = std::time(0);
            struct tm *tm = std::localtime(&t);
            static char result[20];

            std::sprintf(
                result, "%.4d-%.2d-%.2d %.2d:%.2d:%.2d",
                1900 + tm->tm_year, tm->tm_mon + 1, tm->tm_mday,
                tm->tm_hour, tm->tm_min, tm->tm_sec
            );

            return result;
        }

        template <class Log, class... Args>
        void log_format(Log& log, const std::string& format, Args&&... args)
        {
            log << fmt::format("[{}] [MumbleLink]: {}\n",
                getTime(), fmt::format(format, std::forward<Args>(args)...)
            );
        }
    }

    MumbleLink& MumbleLink::getInstance()
    {
        static MumbleLink instance;
        return instance;
    }

    MumbleLink::MumbleLink()
        : log_{"mumbleLog.txt"}
        , cell_{nullptr}
        , cellOffset_{0}
    {
#ifdef _WIN32
        mapHandle_ = OpenFileMappingW(FILE_MAP_ALL_ACCESS, FALSE, L"MumbleLink");
        if (mapHandle_ == nullptr)
            return;

        lm_ = reinterpret_cast<LinkedMem *>(MapViewOfFile(mapHandle_, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(LinkedMem)));
        if (lm_ == nullptr)
        {
            CloseHandle(mapHandle_);
            mapHandle_ = nullptr;
            return;
        }

    #else
        char memname[256];
        snprintf(memname, 256, "/MumbleLink.%d", getuid());

        int shmfd = shm_open(memname, O_RDWR, S_IRUSR | S_IWUSR);

        if (shmfd < 0)
            return;

        lm_ = reinterpret_cast<LinkedMem*>((mmap(NULL, sizeof(struct LinkedMem), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0)));

        if (lm_ == reinterpret_cast<void*>(-1))
        {
            lm_ = nullptr;
            return;
        }
    #endif
    }

    MumbleLink::~MumbleLink()
    {
        if (lm_ == nullptr)
            return;

#ifdef _WIN32
        UnmapViewOfFile(lm_);
        CloseHandle(mapHandle_);
#endif
    }

    void MumbleLink::setCell(const ESM::Cell& cell)
    {
        cell_ = &cell;
        cellOffset_ = hashCell(cell);

        log_format(log_, "Cell name = {}", cell.mName);
        log_format(log_, "Cell grid position = ({}, {})", cell.getGridX(), cell.getGridY());
        log_format(log_, "Hash({}) = {}", cell.mName, hashCell(cell));
    }

    void MumbleLink::setContext(const std::string &context)
    {
        if (lm_ == nullptr)
            return;

        // Context should be equal for players which should be able to hear each other positional and
        // differ for those who shouldn't (e.g. it could contain the server+port and team)
        std::size_t len = std::min(256, static_cast<int>(context.size()));

        std::memcpy(lm_->context, context.c_str(), len);
        lm_->context_len = static_cast<std::uint32_t>(len);

        log_format(log_, "context set = " + context);
    }

    void MumbleLink::setIdentity(const std::string &identity)
    {
        if (lm_ == nullptr)
            return;

        // Identifier which uniquely identifies a certain player in a context (e.g. the ingame name).
        wcsncpy(lm_->identity, std::wstring(identity.begin(), identity.end()).c_str(), 256);
        log_format(log_, "identity set = " + identity);
    }

    void MumbleLink::updateMumble(const osg::Vec3f &pos, const osg::Vec3f &forward, const osg::Vec3f &up)
    {
        if (!lm_)
            return;

        if (lm_->uiVersion != 2)
        {
            wcsncpy(lm_->name, L"TES3MP", 256);
            wcsncpy(lm_->description, L"Supports TES3MP.", 2048);
            lm_->uiVersion = 2;
        }

        lm_->uiTick++;

        osg::Vec3f front = { forward.x(), forward.z(), forward.y() };
        osg::Vec3f top = { up.x(), up.z(), up.y() };
        osg::Vec3f position = { cellOffset_ + pos.x(), cellOffset_ + pos.z(), cellOffset_ + pos.y() };

        // Left handed coordinate system.
        // X positive towards "right".
        // Y positive towards "up".
        // Z positive towards "front".
        //
        // 1 unit = 1 meter

        // Unit vector pointing out of the avatar's eyes aka "At"-vector.
        lm_->fAvatarFront = front;

        // Unit vector pointing out of the top of the avatar's head aka "Up"-vector (here Top points straight up).
        lm_->fAvatarTop = top;

        // Position of the avatar (here standing slightly off the origin)
        lm_->fAvatarPosition = position * convert_to_meters;

        // Same as avatar but for the camera.
        lm_->fCameraPosition = position * convert_to_meters;
        lm_->fCameraFront = front;
        lm_->fCameraTop = top;
    }

    float MumbleLink::hashCell(const ESM::Cell& cell)
    {
        if (cell.isExterior())
            return 0;

        const auto hash = std::hash<std::string>{}(cell.mName);
        return static_cast<short>(std::numeric_limits<unsigned short>::max() - static_cast<unsigned short>(hash));
    }
}
