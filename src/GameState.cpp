//|||||||||||||||||||||||||||||||||||||||||||||||

#include "GameState.hpp"

//|||||||||||||||||||||||||||||||||||||||||||||||

using namespace Ogre;

//|||||||||||||||||||||||||||||||||||||||||||||||

GameState::GameState() {

    m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pRoot->createSceneManager(ST_GENERIC, "GameSceneMgr");

    this->worldBounds = AxisAlignedBox(Ogre::Vector3(-10000, -10000, -10000), Ogre::Vector3(10000, 10000, 10000)); //aligned box for Bullet
    this->gravityVector = Vector3(0, -9.81, 0); // gravity vector for Bullet

    this->mNumEntitiesInstanced = 0; // how many shapes are created
    // Start Bullet
    this->mWorld = new OgreBulletDynamics::DynamicsWorld(m_pSceneMgr, this->worldBounds, this->gravityVector);

    // add Debug info display tool
    debugDrawer = new OgreBulletCollisions::DebugDrawer();
    debugDrawer->setDrawWireframe(true); // we want to see the Bullet containers

    mWorld->setDebugDrawer(debugDrawer);
    mWorld->setShowDebugShapes(true); // enable it if you want to see the Bullet containers
    SceneNode *debugDrawNode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("debugDrawer", Ogre::Vector3::ZERO);
    debugDrawNode->attachObject(static_cast<SimpleRenderable *> (debugDrawer));

    // Define a floor plane mesh
    Entity* floorEnt;
    Plane p;
    p.normal = Vector3(0, 1, 0);
    p.d = 0;
    MeshManager::getSingleton().createPlane("FloorPlane",
            ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
            p, 200000, 200000, 20, 20, true, 1, 9000, 9000,
            Vector3::UNIT_Z);
    // Create an entity (the floor)
    floorEnt = m_pSceneMgr->createEntity("floor", "FloorPlane");
    floorEnt->setMaterialName("Examples/BumpyMetal");
    m_pSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(floorEnt);

    // add collision detection to it
    OgreBulletCollisions::CollisionShape* Shape;
    Shape = new OgreBulletCollisions::StaticPlaneCollisionShape(Ogre::Vector3(0, 1, 0), 0); // (normal vector, distance)
    // a body is needed for the shape
    OgreBulletDynamics::RigidBody* defaultPlaneBody = new OgreBulletDynamics::RigidBody("BasePlane", mWorld);
    defaultPlaneBody->setStaticShape(Shape, 0.1, 0.8); // (shape, restitution, friction)
    // push the created objects to the deques
    mShapes.push_back(Shape);
    mBodies.push_back(defaultPlaneBody);


    this->m_MoveSpeed = 0.1f;
    this->m_RotateSpeed = 0.3f;

    this->m_bLMouseDown = false;
    this->m_bRMouseDown = false;
    this->m_bQuit = false;
    this->m_bSettingsMode = false;

    this->m_pCurrentObject = 0;
    this->m_pDetailsPanel = 0;
}

// Clean up

GameState::~GameState() {
    // OgreBullet physic delete - RigidBodies
    std::deque<OgreBulletDynamics::RigidBody *>::iterator itBody = mBodies.begin();
    while (mBodies.end() != itBody) {
        delete *itBody;
        ++itBody;
    }
    // OgreBullet physic delete - Shapes
    std::deque<OgreBulletCollisions::CollisionShape *>::iterator itShape = mShapes.begin();
    while (mShapes.end() != itShape) {
        delete *itShape;
        ++itShape;
    }
    mBodies.clear();
    mShapes.clear();
    delete mWorld->getDebugDrawer();
    mWorld->setDebugDrawer(0);
    delete mWorld;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::enter() {
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering GameState...");

    m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));

    //    m_pRSQ = m_pSceneMgr->createRayQuery(Ray());
    //    m_pRSQ->setQueryMask(OGRE_HEAD_MASK);

    m_pCamera = m_pSceneMgr->createCamera("GameCamera");
    m_pCamera->setPosition(Vector3(5, 60, 60));
    m_pCamera->lookAt(Vector3(5, 20, 0));
    m_pCamera->setNearClipDistance(5);

    m_pCamera->setAspectRatio(Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualWidth()) /
            Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualHeight()));

    OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);

    buildGUI();

    createScene();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState::pause() {
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Pausing GameState...");

    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::resume() {
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Resuming GameState...");

    buildGUI();

    OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);
    m_bQuit = false;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::exit() {
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Leaving GameState...");

    m_pSceneMgr->destroyCamera(m_pCamera);
    m_pSceneMgr->destroyQuery(m_pRSQ);
    if (m_pSceneMgr)
        OgreFramework::getSingletonPtr()->m_pRoot->destroySceneManager(m_pSceneMgr);
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::createScene() {
    m_pSceneMgr->createLight("Light")->setPosition(75, 75, 75);

    m_pSceneMgr->setSkyBox(true, "darkSea/darkSeaSkyBox");

    m_pCamera->setNearClipDistance(0.1);
    DotSceneLoader* pDotSceneLoader = new DotSceneLoader();
    pDotSceneLoader->parseDotScene("testLevel.xml", "testLevel", m_pSceneMgr, m_pSceneMgr->getRootSceneNode());
    //        pDotSceneLoader->parseDotScene("CubeScene.xml", "General", m_pSceneMgr, m_pSceneMgr->getRootSceneNode());
    delete pDotSceneLoader;

    SoundManager *soundMgr;
    soundMgr = new SoundManager;
    soundMgr->Initialize();
    int soundFireGun;
    String* soundLocation = new String("/home/dahoc/projects/ogrefps/media/sounds/Octopus.wav");
    soundFireGun = soundMgr->CreateSound(*soundLocation);

    int channelFireGun;
    soundMgr->PlaySound(soundFireGun, m_pSceneMgr->getRootSceneNode(), &channelFireGun);
    /*
        m_pSceneMgr->getEntity("Cube01")->setQueryFlags(CUBE_MASK);
        m_pSceneMgr->getEntity("Cube02")->setQueryFlags(CUBE_MASK);
        m_pSceneMgr->getEntity("Cube03")->setQueryFlags(CUBE_MASK);
        m_pOgreHeadEntity = m_pSceneMgr->createEntity("Cube", "ogrehead.mesh");
        m_pOgreHeadEntity->setQueryFlags(OGRE_HEAD_MASK);
        m_pOgreHeadNode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("CubeNode");
        m_pOgreHeadNode->attachObject(m_pOgreHeadEntity);
        m_pOgreHeadNode->setPosition(Vector3(0, 0, -25));

        m_pOgreHeadMat = m_pOgreHeadEntity->getSubEntity(1)->getMaterial();
        m_pOgreHeadMatHigh = m_pOgreHeadMat->clone("OgreHeadMatHigh");
        m_pOgreHeadMatHigh->getTechnique(0)->getPass(0)->setAmbient(1, 0, 0);
        m_pOgreHeadMatHigh->getTechnique(0)->getPass(0)->setDiffuse(1, 0, 0, 0);
     */


}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState::keyPressed(const OIS::KeyEvent &keyEventRef) {
    if (m_bSettingsMode == true) {
        if (OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_S)) {
            OgreBites::SelectMenu* pMenu = (OgreBites::SelectMenu*)OgreFramework::getSingletonPtr()->m_pTrayMgr->getWidget("ChatModeSelMenu");
            if (pMenu->getSelectionIndex() + 1 < (int) pMenu->getNumItems())
                pMenu->selectItem(pMenu->getSelectionIndex() + 1);
        }

        if (OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_W)) {
            OgreBites::SelectMenu* pMenu = (OgreBites::SelectMenu*)OgreFramework::getSingletonPtr()->m_pTrayMgr->getWidget("ChatModeSelMenu");
            if (pMenu->getSelectionIndex() - 1 >= 0)
                pMenu->selectItem(pMenu->getSelectionIndex() - 1);
        }
    }

    if (OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_ESCAPE)) {
        pushAppState(findByName("PauseState"));
        return true;
    }

    if (OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_I)) {
        if (m_pDetailsPanel->getTrayLocation() == OgreBites::TL_NONE) {
            OgreFramework::getSingletonPtr()->m_pTrayMgr->moveWidgetToTray(m_pDetailsPanel, OgreBites::TL_TOPLEFT, 0);
            m_pDetailsPanel->show();
        } else {
            OgreFramework::getSingletonPtr()->m_pTrayMgr->removeWidgetFromTray(m_pDetailsPanel);
            m_pDetailsPanel->hide();
        }
    }

    if (OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_TAB)) {
        m_bSettingsMode = !m_bSettingsMode;
        return true;
    }

    if (m_bSettingsMode && OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_RETURN) ||
            OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_NUMPADENTER)) {
    }

    if (!m_bSettingsMode || (m_bSettingsMode && !OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_O)))
        OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);

    // create and throw a box if 'B' is pressed
    if (OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_B)) {
        Vector3 size = Vector3(2,2,2); // size of the box
        // starting position of the box
        Vector3 position = (m_pCamera->getDerivedPosition() + m_pCamera->getDerivedDirection().normalisedCopy() * 10);

        // create an ordinary, Ogre mesh with texture
        Entity *entity = m_pSceneMgr->createEntity(
                "Box" + StringConverter::toString(mNumEntitiesInstanced),
                "Cube01.mesh");

        entity->setCastShadows(true);
        // we need the bounding box of the box to be able to set the size of the Bullet-box
        AxisAlignedBox boundingB = entity->getBoundingBox();

        size = boundingB.getSize();
        size /= 2.0f; // only the half needed
        size *= 0.95f; // Bullet margin is a bit bigger so we need a smaller size
        // (Bullet 2.76 Physics SDK Manual page 18)
        entity->setMaterialName("Examples/BumpyMetal");

        SceneNode *node = m_pSceneMgr->getRootSceneNode()->createChildSceneNode();
        node->attachObject(entity);
        node->scale(0.05f, 0.05f, 0.05f); // the cube is too big for us
        size *= 0.05f; // don't forget to scale down the Bullet-box too

        // after that create the Bullet shape with the calculated size
        OgreBulletCollisions::BoxCollisionShape *sceneBoxShape = new OgreBulletCollisions::BoxCollisionShape(size);

        // and the Bullet rigid body
        OgreBulletDynamics::RigidBody *defaultBody = new OgreBulletDynamics::RigidBody(
                "defaultBoxRigid" + StringConverter::toString(mNumEntitiesInstanced),
                mWorld);

        defaultBody->setShape(node,
                sceneBoxShape,
                0.6f, // dynamic body restitution
                0.6f, // dynamic body friction
                1.0f, // dynamic bodymass
                position, // starting position of the box
                Quaternion(0, 0, 0, 1)); // orientation of the box

        mNumEntitiesInstanced++;

        defaultBody->setLinearVelocity(
                m_pCamera->getDerivedDirection().normalisedCopy() * 7.0f); // shooting speed

        // push the created objects to the dequese
        mShapes.push_back(sceneBoxShape);
        mBodies.push_back(defaultBody);
    }


    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState::keyReleased(const OIS::KeyEvent &keyEventRef) {
    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState::mouseMoved(const OIS::MouseEvent &evt) {
    if (OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseMove(evt)) return true;

    if (m_bRMouseDown) {
        m_pCamera->yaw(Degree(evt.state.X.rel * -0.1f));
        m_pCamera->pitch(Degree(evt.state.Y.rel * -0.1f));
    }

    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id) {
    if (OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseDown(evt, id)) return true;

    if (id == OIS::MB_Left) {
        onLeftPressed(evt);
        m_bLMouseDown = true;
    } else if (id == OIS::MB_Right) {
        m_bRMouseDown = true;
    }

    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id) {
    if (OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseUp(evt, id)) return true;

    if (id == OIS::MB_Left) {
        m_bLMouseDown = false;
    } else if (id == OIS::MB_Right) {
        m_bRMouseDown = false;
    }

    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::onLeftPressed(const OIS::MouseEvent &evt) {
    if (m_pCurrentObject) {
        m_pCurrentObject->showBoundingBox(false);
        m_pCurrentEntity->getSubEntity(1)->setMaterial(m_pOgreHeadMat);
    }

    Ogre::Ray mouseRay = m_pCamera->getCameraToViewportRay(OgreFramework::getSingletonPtr()->m_pMouse->getMouseState().X.abs / float(evt.state.width),
            OgreFramework::getSingletonPtr()->m_pMouse->getMouseState().Y.abs / float(evt.state.height));
    m_pRSQ->setRay(mouseRay);
    m_pRSQ->setSortByDistance(true);

    Ogre::RaySceneQueryResult &result = m_pRSQ->execute();
    Ogre::RaySceneQueryResult::iterator itr;

    for (itr = result.begin(); itr != result.end(); itr++) {
        if (itr->movable) {
            OgreFramework::getSingletonPtr()->m_pLog->logMessage("MovableName: " + itr->movable->getName());
            m_pCurrentObject = m_pSceneMgr->getEntity(itr->movable->getName())->getParentSceneNode();
            OgreFramework::getSingletonPtr()->m_pLog->logMessage("ObjName " + m_pCurrentObject->getName());
            m_pCurrentObject->showBoundingBox(true);
            m_pCurrentEntity = m_pSceneMgr->getEntity(itr->movable->getName());
            m_pCurrentEntity->getSubEntity(1)->setMaterial(m_pOgreHeadMatHigh);
            break;
        }
    }
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::moveCamera() {
    if (OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_LSHIFT))
        m_pCamera->moveRelative(m_TranslateVector);
    m_pCamera->moveRelative(m_TranslateVector / 10);
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::getInput() {
    if (m_bSettingsMode == false) {
        if (OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_A))
            m_TranslateVector.x = -m_MoveScale;

        if (OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_D))
            m_TranslateVector.x = m_MoveScale;

        if (OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_W))
            m_TranslateVector.z = -m_MoveScale;

        if (OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_S))
            m_TranslateVector.z = m_MoveScale;
    }
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::update(double timeSinceLastFrame) {
    m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
    OgreFramework::getSingletonPtr()->m_pTrayMgr->frameRenderingQueued(m_FrameEvent);

    mWorld->stepSimulation(m_FrameEvent.timeSinceLastFrame); // update Bullet Physics animation

    if (m_bQuit == true) {
        popAppState();
        return;
    }

    if (!OgreFramework::getSingletonPtr()->m_pTrayMgr->isDialogVisible()) {
        if (m_pDetailsPanel->isVisible()) {
            m_pDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(m_pCamera->getDerivedPosition().x));
            m_pDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(m_pCamera->getDerivedPosition().y));
            m_pDetailsPanel->setParamValue(2, Ogre::StringConverter::toString(m_pCamera->getDerivedPosition().z));
            m_pDetailsPanel->setParamValue(3, Ogre::StringConverter::toString(m_pCamera->getDerivedOrientation().w));
            m_pDetailsPanel->setParamValue(4, Ogre::StringConverter::toString(m_pCamera->getDerivedOrientation().x));
            m_pDetailsPanel->setParamValue(5, Ogre::StringConverter::toString(m_pCamera->getDerivedOrientation().y));
            m_pDetailsPanel->setParamValue(6, Ogre::StringConverter::toString(m_pCamera->getDerivedOrientation().z));
            if (m_bSettingsMode)
                m_pDetailsPanel->setParamValue(7, "Buffered Input");
            else
                m_pDetailsPanel->setParamValue(7, "Un-Buffered Input");
        }
    }

    m_MoveScale = m_MoveSpeed * timeSinceLastFrame;
    m_RotScale = m_RotateSpeed * timeSinceLastFrame;

    m_TranslateVector = Vector3::ZERO;

    getInput();
    moveCamera();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::buildGUI() {
    OgreFramework::getSingletonPtr()->m_pTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    OgreFramework::getSingletonPtr()->m_pTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
    OgreFramework::getSingletonPtr()->m_pTrayMgr->createLabel(OgreBites::TL_TOP, "GameLbl", "Game mode", 250);
    OgreFramework::getSingletonPtr()->m_pTrayMgr->showCursor();

    Ogre::StringVector items;
    items.push_back("cam.pX");
    items.push_back("cam.pY");
    items.push_back("cam.pZ");
    items.push_back("cam.oW");
    items.push_back("cam.oX");
    items.push_back("cam.oY");
    items.push_back("cam.oZ");
    items.push_back("Mode");

    m_pDetailsPanel = OgreFramework::getSingletonPtr()->m_pTrayMgr->createParamsPanel(OgreBites::TL_TOPLEFT, "DetailsPanel", 200, items);
    m_pDetailsPanel->show();

    Ogre::String infoText = "[TAB] - Switch input mode\n\n[W] - Forward / Mode up\n[S] - Backwards/ Mode down\n[A] - Left\n";
    infoText.append("[D] - Right\n\nPress [SHIFT] to move faster\n\n[O] - Toggle FPS / logo\n");
    infoText.append("[Print] - Take screenshot\n\n[ESC] - Exit");
    OgreFramework::getSingletonPtr()->m_pTrayMgr->createTextBox(OgreBites::TL_RIGHT, "InfoPanel", infoText, 300, 220);

    Ogre::StringVector chatModes;
    chatModes.push_back("Solid mode");
    chatModes.push_back("Wireframe mode");
    chatModes.push_back("Point mode");
    OgreFramework::getSingletonPtr()->m_pTrayMgr->createLongSelectMenu(OgreBites::TL_TOPRIGHT, "ChatModeSelMenu", "ChatMode", 200, 3, chatModes);
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::itemSelected(OgreBites::SelectMenu* menu) {
    switch (menu->getSelectionIndex()) {
        case 0:
            m_pCamera->setPolygonMode(Ogre::PM_SOLID);
            break;
        case 1:
            m_pCamera->setPolygonMode(Ogre::PM_WIREFRAME);
            break;
        case 2:
            m_pCamera->setPolygonMode(Ogre::PM_POINTS);
            break;
    }
}

//|||||||||||||||||||||||||||||||||||||||||||||||