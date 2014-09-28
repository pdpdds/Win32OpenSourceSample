#ifndef __LOADSCENE_EXAMPLE_H_INCLUDED__
#define __LOADSCENE_EXAMPLE_H_INCLUDED__

#include "exampleframework.h"
#include <irrString.h>
#include <irrTypes.h>


class IRigidBody;

class CLoadSceneExample : public CExampleFramework
{
    public:
        CLoadSceneExample();
        virtual ~CLoadSceneExample();

        virtual void runExample();

        virtual bool OnEvent(const irr::SEvent& event);
};

#endif // __HELLO_WORLD_EXAMPLE_H_INCLUDED__
