#ifndef OPENMW_MUMBLELINK_HPP
#define OPENMW_MUMBLELINK_HPP

#include <osg/Vec3f>
#include <string>
#include <fstream>

#ifdef _WIN32
#include <windows.h>
#endif

struct LinkedMem
{
#ifdef WIN32
    UINT32  uiVersion;
    DWORD   uiTick;
#else
    uint32_t uiVersion;
    uint32_t uiTick;
#endif
    osg::Vec3f fAvatarPosition;
    osg::Vec3f fAvatarFront;
    osg::Vec3f fAvatarTop;
    wchar_t name[256];
    osg::Vec3f fCameraPosition;
    osg::Vec3f fCameraFront;
    osg::Vec3f fCameraTop;
    wchar_t identity[256];
#ifdef WIN32
    UINT32  context_len;
#else
    uint32_t context_len;
#endif
    unsigned char context[256];
    wchar_t description[2048];
};

namespace mwmp
{
    class MumbleLink
    {
    public:
        static constexpr float convert_to_meters = 0.0075f;

        static MumbleLink& getInstance();

    public:
        void setContext(const std::string &context);
        void setIdentity(const std::string &identity);
        void updateMumble(const osg::Vec3f &pos, const osg::Vec3f &forward, const osg::Vec3f &up);

        void log(const std::string& text);

        MumbleLink(const MumbleLink&) = delete;
        void operator=(const MumbleLink&) = delete;

        ~MumbleLink();

    private:
        MumbleLink();

    private:
        LinkedMem* lm_;
        std::ofstream log_;
#ifdef _WIN32
        HANDLE mapHandle_;
#endif
    };
}

#endif
