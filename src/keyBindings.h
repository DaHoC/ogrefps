/* 
 * File:   keyBindings.h
 * Author: dahoc
 *
 * Created on 22. November 2010, 21:18
 */

#ifndef KEYBINDINGS_H
#define	KEYBINDINGS_H

/**
 @TODO: Different modes (walk...), parse from xml file, default mapping*/
class keyBindings {

private:
    std::map<OIS::KeyCode, std::string> keyMapping;


public:
    keyBindings() {
        // init with default values
        this->keyMapping.insert(OIS::KC_W,"forward");

        /// @TODO: Load from XML file

    }

};

#endif	/* KEYBINDINGS_H */

