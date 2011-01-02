/* 
 * File:   player.cpp
 * Author: dahoc
 * 
 * Created on 15. November 2010, 13:09
 */

#include "player.h"

player::player(Ogre::SceneManager* m_pSceneMgr, Ogre::Camera* m_pCamera) {
    this->m_pSceneMgr = m_pSceneMgr;
    this->m_pCamera = m_pCamera;

    this->m_pCamera->setPosition(Ogre::Vector3(0, 5, 0));
    this->m_pCamera->lookAt(Ogre::Vector3(0, 5, 0));
    this->m_pCamera->setNearClipDistance(1);

    this->m_pCamera->setAspectRatio(Ogre::Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualWidth()) / Ogre::Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualHeight()));

    OgreFramework::getSingletonPtr()->m_pViewport->setCamera(this->m_pCamera);

    // Create the camera's top node (which will only handle position).
    this->cameraNode = this->m_pSceneMgr->getRootSceneNode()->createChildSceneNode();
    this->cameraNode->setPosition(0, 0, 0);

//    this->cameraNode->attachObject(this->m_pCamera);

    // Create the camera's yaw node as a child of camera's top node
    this->cameraYawNode = this->cameraNode->createChildSceneNode();

    // Create the camera's pitch node as a child of camera's yaw node
    this->cameraPitchNode = this->cameraYawNode->createChildSceneNode();

    // Create the camera's roll node as a child of camera's pitch node
    // and attach the camera to it.
    this->cameraRollNode = this->cameraPitchNode->createChildSceneNode();

    this->cameraRollNode->attachObject(this->m_pCamera);

    this->moveSpeed = 0.1f;
    this->rotateSpeed = 0.3f;
}

// rotate around x-axis translated by current player node position
void player::pitch(const Ogre::Radian& amount) {
    /// @TODO: Limit amount to less or equal +- 90 degrees
    Ogre::Vector3 rotationAxis = this->cameraPitchNode->getOrientation() * Ogre::Vector3::UNIT_X;
//    rotationAxis.y = 0.;
//    rotationAxis.z = 0.;
    this->rotationsQuaternion = Ogre::Quaternion(amount, rotationAxis);
//    this->rotationsQuaternion.y = 0.;
//    this->rotationsQuaternion.z = 0.;
    this->rotationsQuaternion.normalise(); // remember to normalise to avoid unwanted & unprecise results
    this->cameraPitchNode->rotate(this->rotationsQuaternion, Ogre::SceneNode::TS_LOCAL);
}
/*
    /// @TODO: Correct angle, the rotation is not around the center (but around x-axis??)
    Ogre::Real pitchAngle = (2 * (Ogre::Degree(Ogre::Math::ACos(this->cameraPitchNode->getOrientation().w))).valueDegrees());

    // Just to determine the sign of the angle we pick up above, the
    // value itself does not interest us.
    Ogre::Real pitchAngleSign = this->cameraPitchNode->getOrientation().x;

    /// @TODO: @fixme: Limit the pitch between > -90 degress and < +90 degrees, Quake3-style.
    if (pitchAngle > 90.0f) {
        if (pitchAngleSign > 0)
            // Set orientation to 90 degrees on X-axis.
            this->cameraPitchNode->setOrientation(Ogre::Quaternion(Ogre::Math::Sqrt(0.5f), Ogre::Math::Sqrt(0.5f), 0, 0));
        else if (pitchAngleSign < 0)
            // Sets orientation to -90 degrees on X-axis.
            this->cameraPitchNode->setOrientation(Ogre::Quaternion(Ogre::Math::Sqrt(0.5f), -Ogre::Math::Sqrt(0.5f), 0, 0));
    } else
        this->cameraPitchNode->pitch(amount);
*/

// rotate around y-axis translated by current player node position
void player::yaw(const Ogre::Radian& amount) {
    Ogre::Vector3 rotationAxis = this->cameraYawNode->getOrientation() * Ogre::Vector3::UNIT_Y;
//    rotationAxis.x = 0.;
//    rotationAxis.z = 0.;
    this->rotationsQuaternion = Ogre::Quaternion(amount, rotationAxis);
//    this->rotationsQuaternion.x = 0.;
//    this->rotationsQuaternion.z = 0.;
    this->rotationsQuaternion.normalise(); // remember to normalise to avoid unwanted & unprecise results
    this->cameraYawNode->rotate(this->rotationsQuaternion, Ogre::SceneNode::TS_LOCAL);
}

// rotate around z-axis translated by current player node position
void player::roll(const Ogre::Radian& amount) {
    Ogre::Vector3 rotationAxis = this->cameraRollNode->getOrientation() * Ogre::Vector3::UNIT_Z;
    this->rotationsQuaternion = Ogre::Quaternion(amount, rotationAxis);
    this->rotationsQuaternion.normalise(); // remember to normalise to avoid unwanted & unprecise results
    this->cameraRollNode->rotate(this->rotationsQuaternion, Ogre::SceneNode::TS_LOCAL);
}

Ogre::Camera* player::getCamera() {
    return this->m_pCamera;
}

void player::translate(const Ogre::Real& x, const Ogre::Real& y, Ogre::Real& z, Ogre::SceneNode::TransformSpace ts) {
    this->translate(Ogre::Vector3(x, y, z), ts); // forward to correct function
//    this->cameraNode->translate(x, y, z, ts);
}

/// @TODO: Move cameraRollNode or m_pCamera directly, but yaw/pitch/roll using quaternions on the same node (because otherwise/currently, the rotation center does not move along)
void player::translate(const Ogre::Vector3& translateVector, Ogre::SceneNode::TransformSpace ts) {
//    Ogre::Vector3* forwardViewVector = new Ogre::Vector3(this->m_pCamera->getDerivedDirection().x, this->m_pCamera->getDerivedDirection().y, this->m_pCamera->getDerivedDirection().z);
//    Ogre::Quaternion* rotAroundArbitratyAxis = new Ogre::Quaternion(0,0,0,1);
//    this->cameraNode->translate(*tVector, ts);
    // move relative to yaw (node), forward means -z in yaw nodes coordinates
    this->cameraNode->translate(this->cameraYawNode->getOrientation() *
                             this->cameraPitchNode->getOrientation() *
                             translateVector,
                             Ogre::SceneNode::TS_LOCAL);

//    this->cameraYawNode->translate(translateVector, ts);
//    this->cameraNode->translate(translateVector, ts);
}

Ogre::Real player::getMoveSpeed() {
    return this->moveSpeed;
}

Ogre::Real player::getRotateSpeed() {
    return this->rotateSpeed;
}

player::~player() {
}

