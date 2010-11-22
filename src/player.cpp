/* 
 * File:   player.cpp
 * Author: dahoc
 * 
 * Created on 15. November 2010, 13:09
 */

#include "player.h"

player::player(Ogre::SceneManager* m_pSceneMgr, Ogre::Camera* m_pCamera) {
    // Create the camera's top node (which will only handle position).
    this->cameraNode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode();
    this->cameraNode->setPosition(0, 0, 0);

    // Create the camera's yaw node as a child of camera's top node.
    this->cameraYawNode = this->cameraNode->createChildSceneNode();

    // Create the camera's pitch node as a child of camera's yaw node.
    this->cameraPitchNode = this->cameraYawNode->createChildSceneNode();

    // Create the camera's roll node as a child of camera's pitch node
    // and attach the camera to it.
    this->cameraRollNode = this->cameraPitchNode->createChildSceneNode();
    this->cameraRollNode->attachObject(this->m_pCamera);

    this->moveSpeed = 0.1f;
    this->rotateSpeed = 0.3f;
}

void player::pitch(const Ogre::Radian amount) {
    // Angle of rotation around the X-axis.
    Ogre::Real pitchAngle = (2 * (Ogre::Degree(Ogre::Math::ACos(this->cameraPitchNode->getOrientation().w))).valueDegrees());

    // Just to determine the sign of the angle we pick up above, the
    // value itself does not interest us.
    Ogre::Real pitchAngleSign = this->cameraPitchNode->getOrientation().x;

    // Limit the pitch between -80 degress and +80 degrees, Quake3-style.
    if (pitchAngle > 80.0f) {
        if (pitchAngleSign > 0)
            // Set orientation to 90 degrees on X-axis.
            this->cameraPitchNode->setOrientation(Ogre::Quaternion(Ogre::Math::Sqrt(0.5f), Ogre::Math::Sqrt(0.5f), 0, 0));
        else if (pitchAngleSign < 0)
            // Sets orientation to -90 degrees on X-axis.
            this->cameraPitchNode->setOrientation(Ogre::Quaternion(Ogre::Math::Sqrt(0.5f), -Ogre::Math::Sqrt(0.5f), 0, 0));
    } else
        this->cameraPitchNode->pitch(amount);
}

void player::yaw(const Ogre::Radian amount) {
    this->cameraYawNode->yaw(amount);
}

void player::roll(const Ogre::Radian amount) {
    this->cameraRollNode->roll(amount);
}

void player::translate(Ogre::Real x, Ogre::Real y, Ogre::Real z, Ogre::SceneNode::TransformSpace ts) {
    this->cameraNode->translate(x, y, z, ts);
}

Ogre::Real player::getMoveSpeed() {
    return this->moveSpeed;
}

Ogre::Real player::getRotateSpeed() {
    return this->rotateSpeed;
}

Ogre::Vector3 player::getTranslateVector() {
    return this->translateVector;
}

player::~player() {
}
