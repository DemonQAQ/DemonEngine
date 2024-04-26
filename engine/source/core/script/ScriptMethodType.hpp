//
// Created by Demon on 2024/4/26.
//

#ifndef DEMONENGINE_SCRIPTMETHODTYPE_HPP
#define DEMONENGINE_SCRIPTMETHODTYPE_HPP

namespace script
{
    enum ScriptMethodType
    {
        ON_UPDATE,
        ON_PHYSICS
    };

    inline const char *toString(ScriptMethodType type)
    {
        switch (type)
        {
            case ScriptMethodType::ON_UPDATE:
                return "onUpdate";
            case ScriptMethodType::ON_PHYSICS:
                return "onUpdate";
            default:
                return "";
        }
    }
}
#endif //DEMONENGINE_SCRIPTMETHODTYPE_HPP
