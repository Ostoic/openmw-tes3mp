#include <components/openmw-mp/NetworkMessages.hpp>
#include "PacketConsoleCommand.hpp"

using namespace mwmp;

PacketConsoleCommand::PacketConsoleCommand(RakNet::RakPeerInterface *peer) : ObjectPacket(peer)
{
    packetID = ID_CONSOLE_COMMAND;
    hasCellData = true;
}

void PacketConsoleCommand::Packet(RakNet::BitStream *bs, bool send)
{
    if (!PacketHeader(bs, send))
        return;

    RW(objectList->consoleCommand, send);

    BaseObject baseObject;
    for (unsigned int i = 0; i < objectList->baseObjectCount; i++)
    {
        if (send)
            baseObject = objectList->baseObjects.at(i);

        RW(baseObject.isPlayer, send);

        if (baseObject.isPlayer)
            RW(baseObject.guid, send);
        else
            Object(baseObject, send);

        if (!send)
            objectList->baseObjects.push_back(baseObject);
    }
}
