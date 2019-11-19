#ifndef OPENMW_BASEWORLDSTATE_HPP
#define OPENMW_BASEWORLDSTATE_HPP

#include <vector>

#include <components/esm/loadacti.hpp>
#include <components/esm/loadalch.hpp>
#include <components/esm/loadappa.hpp>
#include <components/esm/loadarmo.hpp>
#include <components/esm/loadbook.hpp>
#include <components/esm/loadclot.hpp>
#include <components/esm/loadcont.hpp>
#include <components/esm/loadcrea.hpp>
#include <components/esm/loaddoor.hpp>
#include <components/esm/loadench.hpp>
#include <components/esm/loadingr.hpp>
#include <components/esm/loadlock.hpp>
#include <components/esm/loadmisc.hpp>
#include <components/esm/loadnpc.hpp>
#include <components/esm/loadprob.hpp>
#include <components/esm/loadrepa.hpp>
#include <components/esm/loadspel.hpp>
#include <components/esm/loadstat.hpp>
#include <components/esm/loadweap.hpp>

#include <components/openmw-mp/Base/BaseStructs.hpp>

#include <RakNetTypes.h>

namespace mwmp
{
    enum RECORD_TYPE
    {
        ARMOR,
        BOOK,
        CLOTHING,
        CREATURE,
        ENCHANTMENT,
        MISCELLANEOUS,
        NPC,
        POTION,
        SPELL,
        WEAPON,
        CONTAINER,
        DOOR,
        ACTIVATOR,
        STATIC,
        INGREDIENT,
        APPARATUS,
        LOCKPICK,
        PROBE,
        REPAIR
    };

    // When using an existing record as a base, this struct tracks which changes
    // need to be made to it
    //
    // Note: These can't be replaced with checks for empty strings or numerical
    //       values of 0 because you want to be able to blank out strings or
    //       set values of 0 through overrides, i.e. if someone is in the
    //       Mages Guild faction, you want to be able to remove them from it
    //       by using a blank faction string as an override
    //
    //       There are, however, a few values that are not allowed to be blanked
    //       out in a record, such as races or classes for NPCs, and those
    //       should rely on checks for empty strings instead of having a
    //       boolean here
    struct BaseOverrides
    {
        bool hasSubtype = false;
        bool hasName = false;
        bool hasModel = false;
        bool hasIcon = false;
        bool hasScript = false;
        bool hasEnchantmentId = false;
        bool hasEnchantmentCharge = false;

        bool hasEffects = false;
        bool hasBodyParts = false;
        bool hasInventory = false;

        bool hasAutoCalc = false;
        bool hasCharge = false;
        bool hasCost = false;
        bool hasFlags = false;
        bool hasValue = false;
        bool hasWeight = false;
        bool hasQuality = false;
        bool hasUses = false;

        bool hasArmorRating = false;
        bool hasHealth = false;

        bool hasDamageChop = false;
        bool hasDamageSlash = false;
        bool hasDamageThrust = false;
        bool hasReach = false;
        bool hasSpeed = false;

        bool hasKeyState = false;
        bool hasScrollState = false;
        bool hasSkillId = false;
        bool hasText = false;

        bool hasHair = false;
        bool hasHead = false;
        bool hasGender = false;
        bool hasFaction = false;

        bool hasLevel = false;
        bool hasMagicka = false;
        bool hasFatigue = false;

        bool hasAiFight = false;
        bool hasAiFlee = false;
        bool hasAiAlarm = false;
        bool hasAiServices = false;

        bool hasOpenSound = false;
        bool hasCloseSound = false;
    };

    struct ActivatorRecord
    {
        ESM::Activator data;
        std::string baseId;
        BaseOverrides baseOverrides;
    };

    struct ApparatusRecord
    {
        ESM::Apparatus data;
        std::string baseId;
        BaseOverrides baseOverrides;
    };

    struct ArmorRecord
    {
        ESM::Armor data;
        std::string baseId;
        BaseOverrides baseOverrides;
    };

    struct BookRecord
    {
        ESM::Book data;
        std::string baseId;
        BaseOverrides baseOverrides;
    };

    struct ClothingRecord
    {
        ESM::Clothing data;
        std::string baseId;
        BaseOverrides baseOverrides;
    };

    struct ContainerRecord
    {
        ESM::Container data;
        std::string baseId;
        std::vector<mwmp::Item> inventory;
        BaseOverrides baseOverrides;
    };

    struct CreatureRecord
    {
        ESM::Creature data;
        std::string baseId;
        std::string inventoryBaseId;
        std::vector<mwmp::Item> inventory;
        BaseOverrides baseOverrides;
    };

    struct DoorRecord
    {
        ESM::Door data;
        std::string baseId;
        BaseOverrides baseOverrides;
    };

    struct EnchantmentRecord
    {
        ESM::Enchantment data;
        std::string baseId;
        BaseOverrides baseOverrides;
    };

    struct IngredientRecord
    {
        ESM::Ingredient data;
        std::string baseId;
        BaseOverrides baseOverrides;
    };

    struct LockpickRecord
    {
        ESM::Lockpick data;
        std::string baseId;
        BaseOverrides baseOverrides;
    };

    struct MiscellaneousRecord
    {
        ESM::Miscellaneous data;
        std::string baseId;
        BaseOverrides baseOverrides;
    };

    struct NpcRecord
    {
        ESM::NPC data;
        std::string baseId;
        std::string inventoryBaseId;
        std::vector<mwmp::Item> inventory;
        BaseOverrides baseOverrides;
    };

    struct PotionRecord
    {
        ESM::Potion data;
        std::string baseId;
        BaseOverrides baseOverrides;
    };

    struct ProbeRecord
    {
        ESM::Probe data;
        std::string baseId;
        BaseOverrides baseOverrides;
    };

    struct RepairRecord
    {
        ESM::Repair data;
        std::string baseId;
        BaseOverrides baseOverrides;
    };

    struct SpellRecord
    {
        ESM::Spell data;
        std::string baseId;
        BaseOverrides baseOverrides;
    };

    struct StaticRecord
    {
        ESM::Static data;
        std::string baseId;
        BaseOverrides baseOverrides;
    };

    struct WeaponRecord
    {
        ESM::Weapon data;
        std::string baseId;
        BaseOverrides baseOverrides;
    };

    static const int maxImageDataSize = 1800;

    struct MapTile
    {
        int x;
        int y;
        std::vector<char> imageData;
    };

    struct Weather
    {
        std::string region;
        unsigned int currentWeather;
        unsigned int nextWeather;
        unsigned int queuedWeather;
        float transitionFactor;
    };

    class BaseWorldstate
    {
    public:

        BaseWorldstate()
        {
            time.year = -1;
            time.month = -1;
            time.day = -1;
            time.hour = -1;

            time.daysPassed = -1;
            time.timeScale = -1;
        }

        RakNet::RakNetGUID guid;

        mwmp::Time time;

        bool hasPlayerCollision;
        bool hasActorCollision;
        bool hasPlacedObjectCollision;
        bool useActorCollisionForPlacedObjects;

        std::string authorityRegion;

        std::vector<std::string> enforcedCollisionRefIds;

        std::vector<MapTile> mapTiles;

        bool forceWeather;
        Weather weather;

        unsigned short recordsType;
        unsigned int recordsCount;

        std::vector<ActivatorRecord> activatorRecords;
        std::vector<ApparatusRecord> apparatusRecords;
        std::vector<ArmorRecord> armorRecords;
        std::vector<BookRecord> bookRecords;
        std::vector<ClothingRecord> clothingRecords;
        std::vector<ContainerRecord> containerRecords;
        std::vector<CreatureRecord> creatureRecords;
        std::vector<DoorRecord> doorRecords;
        std::vector<EnchantmentRecord> enchantmentRecords;
        std::vector<IngredientRecord> ingredientRecords;
        std::vector<LockpickRecord> lockpickRecords;
        std::vector<MiscellaneousRecord> miscellaneousRecords;
        std::vector<NpcRecord> npcRecords;
        std::vector<PotionRecord> potionRecords;
        std::vector<ProbeRecord> probeRecords;
        std::vector<RepairRecord> repairRecords;
        std::vector<SpellRecord> spellRecords;
        std::vector<StaticRecord> staticRecords;
        std::vector<WeaponRecord> weaponRecords;

        bool isValid;
    };
}

#endif //OPENMW_BASEWORLDSTATE_HPP
