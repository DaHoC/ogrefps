/*
-----------------------------------------------------------------------------
Filename:    BaseApplication.cpp
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _ 
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/                              
      Tutorial Framework
      http://www.ogre3d.org/tikiwiki/
-----------------------------------------------------------------------------
 */
#include "BaseApplication.h"

//-------------------------------------------------------------------------------------

BaseApplication::BaseApplication(void)
: mRoot(0),
mCamera(0),
mSceneMgr(0),
mWindow(0),
mResourcesCfg(Ogre::StringUtil::BLANK),
mPluginsCfg(Ogre::StringUtil::BLANK),
mTrayMgr(0),
mCameraMan(0),
mDetailsPanel(0),
mCursorWasVisible(false),
mShutDown(false),
mInputManager(0),
mMouse(0),
mKeyboard(0) {
}

//-------------------------------------------------------------------------------------

BaseApplication::~BaseApplication(void) {
    if (mTrayMgr) delete mTrayMgr;
    if (mCameraMan) delete mCameraMan;

    //Remove ourself as a Window listener
    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
    windowClosed(mWindow);
    delete mRoot;
}

//-------------------------------------------------------------------------------------

bool BaseApplication::configure(void) {
    // Show the configuration dialog and initialise the system
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg
    if (mRoot->showConfigDialog()) {
        // If returned true, user clicked OK so initialise
        // Here we choose to let the system create a default rendering window by passing 'true'
        mWindow = mRoot->initialise(true, "ogrefps");

        return true;
    } else {
        return false;
    }
}
//-------------------------------------------------------------------------------------

void BaseApplication::chooseSceneManager(void) {
    // Get the SceneManager, in this case a generic one
    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
}
//-------------------------------------------------------------------------------------

void BaseApplication::createCamera(void) {
    // Create the camera
    mCamera = mSceneMgr->createCamera("PlayerCam");

    // Position it at 500 in Z direction
    mCamera->setPosition(Ogre::Vector3(0, 0, 80));
    // Look back along -Z
    mCamera->lookAt(Ogre::Vector3(0, 0, -300));
    mCamera->setNearClipDistance(5);

    mCameraMan = new OgreBites::SdkCameraMan(mCamera); // create a default camera controller
}
//-------------------------------------------------------------------------------------

void BaseApplication::createFrameListener(void) {
#if OGRE_PLATFORM == OGRE_PLATFORM_IPHONE
    mFrameListener = new ExampleFrameListener(mWindow, mCamera, true, true, true);
#else
    mFrameListener = new ExampleFrameListener(mWindow, mCamera);
#endif
    mFrameListener->showDebugOverlay(true);
    mRoot->addFrameListener(mFrameListener);
    // Create the camera's top node (which will only handle position).
    this->playerBoundingBoxNode = this->mSceneMgr->getRootSceneNode()->createChildSceneNode();
    
//    this->cameraNode->setPosition(0, 0, 500);
    this->cameraNode = this->playerBoundingBoxNode->createChildSceneNode();

    // Create the camera's yaw node as a child of camera's top node.
    this->cameraYawNode = this->cameraNode->createChildSceneNode();

    // Create the camera's pitch node as a child of camera's yaw node.
    this->cameraPitchNode = this->cameraYawNode->createChildSceneNode();

    // Create the camera's roll node as a child of camera's pitch node
    // and attach the camera to it.
    this->cameraRollNode = this->cameraPitchNode->createChildSceneNode();
    this->cameraRollNode->attachObject(this->mCamera);
}

//-------------------------------------------------------------------------------------

void BaseApplication::destroyScene(void) {
}
//-------------------------------------------------------------------------------------

void BaseApplication::createViewports(void) {
    // Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));

    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}
//-------------------------------------------------------------------------------------

void BaseApplication::setupResources(void) {
    // Load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load(mResourcesCfg);

    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements()) {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i) {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                    archName, typeName, secName);
        }
    }
}
//-------------------------------------------------------------------------------------

void BaseApplication::createResourceListener(void) {

}
//-------------------------------------------------------------------------------------

void BaseApplication::loadResources(void) {
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}
//-------------------------------------------------------------------------------------

void BaseApplication::go(void) {
#ifdef _DEBUG
    mResourcesCfg = "/home/dahoc/projects/ogrefps/resources/resources_d.cfg";
    mPluginsCfg = "/home/dahoc/projects/ogrefps/resources/plugins_d.cfg";
#else
    mResourcesCfg = "/home/dahoc/projects/ogrefps/resources/resources.cfg";
    mPluginsCfg = "/home/dahoc/projects/ogrefps/resources/plugins.cfg";
#endif

    if (!setup())
        return;

    mRoot->startRendering();

    // clean up
    destroyScene();
}
//-------------------------------------------------------------------------------------

bool BaseApplication::setup(void) {
    mRoot = new Ogre::Root(mPluginsCfg);

    setupResources();

    bool carryOn = configure();
    if (!carryOn) return false;

    chooseSceneManager();
    createCamera();
    createViewports();

    // Set default mipmap level (NB some APIs ignore this)
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    // Create any resource listeners (for loading screens)
    createResourceListener();
    // Load resources
    loadResources();

    // Create the scene
    createScene();

    createFrameListener();

    return true;
};
//-------------------------------------------------------------------------------------

void BaseApplication::createScene(void) {

    mSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox");
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

    Ogre::Light* pointLight = mSceneMgr->createLight("pointLight");
    pointLight->setType(Ogre::Light::LT_POINT);
    pointLight->setPosition(Ogre::Vector3(0, 150, 250));
    pointLight->setDiffuseColour(0.5, 0.5, 0.5);
    pointLight->setSpecularColour(0.8, 0.1, 0.1);

    Ogre::Plane plane(Ogre::Vector3::UNIT_Y, -80);
    Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
            plane, 5000, 5000, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
    Ogre::Entity* entGround = mSceneMgr->createEntity("GroundEntity", "ground");
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entGround);
    entGround->setMaterialName("Examples/Rockwall");
    entGround->setCastShadows(false);

}

bool BaseApplication::frameRenderingQueued(const Ogre::FrameEvent& evt) {
    if (mWindow->isClosed())
        return false;

    if (mShutDown)
        return false;

    mTrayMgr->frameRenderingQueued(evt);

    if (!mTrayMgr->isDialogVisible()) {
        mCameraMan->frameRenderingQueued(evt); // if dialog isn't up, then update the camera
        if (mDetailsPanel->isVisible()) // if details panel is visible, then update its contents
        {
            mDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(mCamera->getDerivedPosition().x));
            mDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(mCamera->getDerivedPosition().y));
            mDetailsPanel->setParamValue(2, Ogre::StringConverter::toString(mCamera->getDerivedPosition().z));
            mDetailsPanel->setParamValue(4, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().w));
            mDetailsPanel->setParamValue(5, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().x));
            mDetailsPanel->setParamValue(6, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().y));
            mDetailsPanel->setParamValue(7, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().z));
        }
    }

    return true;
}
//-------------------------------------------------------------------------------------

void BaseApplication::windowResized(Ogre::RenderWindow* rw) {
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

//Unattach OIS before window shutdown (very important under Linux)

void BaseApplication::windowClosed(Ogre::RenderWindow* rw) {
    //Only close for window that created OIS (the main window in these demos)
    if (rw == mWindow) {
        if (mInputManager) {
            mInputManager->destroyInputObject(mMouse);
            mInputManager->destroyInputObject(mKeyboard);

            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = 0;
        }
    }
}
