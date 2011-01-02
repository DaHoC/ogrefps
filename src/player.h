/* 
 * File:   player.h
 * Author: dahoc
 *
 * Created on 15. November 2010, 13:09
 */

#ifndef PLAYER_H
#define	PLAYER_H

#include "AdvancedOgreFramework.hpp"
#include "keyBindings.h"

//using namespace Ogre;

class player {
public:
    player(Ogre::SceneManager* m_pSceneMgr, Ogre::Camera* m_pCamera);
    virtual ~player();
    Ogre::Real getMoveSpeed();
    Ogre::Real getRotateSpeed();
    Ogre::Camera* getCamera();
    void pitch(const Ogre::Radian& amount);
    void yaw(const Ogre::Radian& amount);
    void roll(const Ogre::Radian& amount);
    void translate(const Ogre::Real& x, const Ogre::Real& y, Ogre::Real& z, Ogre::SceneNode::TransformSpace ts = Ogre::SceneNode::TS_LOCAL);
    void translate(const Ogre::Vector3& translateVector, Ogre::SceneNode::TransformSpace ts = Ogre::SceneNode::TS_LOCAL);

private:
    Ogre::Quaternion rotationsQuaternion;

    Ogre::Real moveSpeed;
    Ogre::Real rotateSpeed;
//    Ogre::Vector3 translateVector;

    Ogre::SceneNode* cameraNode;
    Ogre::SceneNode* cameraYawNode;
    Ogre::SceneNode* cameraPitchNode;
    Ogre::SceneNode* cameraRollNode;

    Ogre::Camera* m_pCamera;
    Ogre::SceneManager* m_pSceneMgr;
};

#endif	/* PLAYER_H */

