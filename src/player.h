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

class player {
public:
    player(Ogre::SceneManager* m_pSceneMgr, Ogre::Camera* m_pCamera);
    virtual ~player();
    Ogre::Real getMoveSpeed();
    Ogre::Real getRotateSpeed();
    Ogre::Vector3 getTranslateVector();
    void pitch(const Ogre::Radian& amount);
    void yaw(const Ogre::Radian& amount);
    void roll(const Ogre::Radian& amount);
    void translate(Ogre::Real x, Ogre::Real y, Ogre::Real z, Ogre::SceneNode::TransformSpace ts);
    void translate(const Ogre::Vector3 translateVector, Ogre::SceneNode::TransformSpace ts);
    
private:
    Ogre::Real moveSpeed;
    Ogre::Real rotateSpeed;
    Ogre::Vector3 translateVector;

    Ogre::SceneNode* cameraNode;
    Ogre::SceneNode* cameraYawNode;
    Ogre::SceneNode* cameraPitchNode;
    Ogre::SceneNode* cameraRollNode;

    Ogre::Camera* m_pCamera;
    Ogre::SceneManager* m_pSceneMgr;
};

#endif	/* PLAYER_H */

