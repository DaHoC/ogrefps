//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

//|||||||||||||||||||||||||||||||||||||||||||||||

#include "AppState.hpp"

#include "DotSceneLoader.hpp"

#include <OgreSubEntity.h>
#include <OgreMaterialManager.h>
#include "SoundManager.h"
#include "player.h"

#include "OgreBulletDynamicsRigidBody.h"				 // for OgreBullet
#include "Shapes/OgreBulletCollisionsStaticPlaneShape.h" // for static planes
#include "Shapes/OgreBulletCollisionsBoxShape.h"		 // for Boxes


//|||||||||||||||||||||||||||||||||||||||||||||||

enum QueryFlags {
    OGRE_HEAD_MASK = 1 << 0,
    CUBE_MASK = 1 << 1
};

//|||||||||||||||||||||||||||||||||||||||||||||||

class GameState : public AppState {
public:
    GameState();
    ~GameState();
    DECLARE_APPSTATE_CLASS(GameState)

    void enter();
    void createScene();
    void exit();
    bool pause();
    void resume();

    void moveCamera();
    void getInput();
    void buildGUI();

    bool keyPressed(const OIS::KeyEvent &keyEventRef);
    bool keyReleased(const OIS::KeyEvent &keyEventRef);

    bool mouseMoved(const OIS::MouseEvent &arg);
    bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

    void onLeftPressed(const OIS::MouseEvent &evt);
    void itemSelected(OgreBites::SelectMenu* menu);

    void update(double timeSinceLastFrame);
    bool loadLevel(unsigned short levelNum);

private:
    SoundManager* soundMgr;
    unsigned int audioId;

    player* firstPerson;
    
    Ogre::SceneNode* m_pOgreHeadNode;
    Ogre::Entity* m_pOgreHeadEntity;
    Ogre::MaterialPtr m_pOgreHeadMat;
    Ogre::MaterialPtr m_pOgreHeadMatHigh;

    OgreBites::ParamsPanel* m_pDetailsPanel;
    bool m_bQuit;

    Ogre::Vector3 m_TranslateVector;
    Ogre::Real m_MoveSpeed;
    Ogre::Degree m_RotateSpeed;
    float m_MoveScale;
    Ogre::Degree m_RotScale;

    Ogre::RaySceneQuery* m_pRSQ;
    Ogre::SceneNode* m_pCurrentObject;
    Ogre::Entity* m_pCurrentEntity;
    bool m_bLMouseDown, m_bRMouseDown;
    bool m_bSettingsMode;

    // Bullet physics stuff
    Ogre::Vector3 gravityVector;
    AxisAlignedBox worldBounds;
    OgreBulletDynamics::DynamicsWorld* mWorld; // OgreBullet World
    OgreBulletCollisions::DebugDrawer* debugDrawer;
    unsigned int mNumEntitiesInstanced;

    std::deque<OgreBulletDynamics::RigidBody *> mBodies;
    std::deque<OgreBulletCollisions::CollisionShape *> mShapes;

};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||