/* 
 * File:   keyBindings.h
 * Author: dahoc
 *
 * Created on 22. November 2010, 21:18
 */

#ifndef KEYBINDINGS_H
#define	KEYBINDINGS_H

/**
 * @TODO: Different modes (walk...), parse from xml file, default mapping
 */
class keyBindings {

private:
    std::map<OIS::KeyCode, std::string> keyMapping;

public:
    keyBindings() {
        // init with default values
        std::pair<OIS::KeyCode, std::string> move_forward = std::make_pair(OIS::KC_W,std::string("forward"));

        this->keyMapping.insert(move_forward);

        /// @TODO: Load from XML file

    }

    std::string getActionForKey(const OIS::KeyCode &keyCode) {
        return this->keyMapping.at(keyCode);
    }

};

#endif	/* KEYBINDINGS_H */

