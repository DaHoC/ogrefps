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

    /// @TODO: Implement me correctly, see http://www.ogre3d.org/forums/viewtopic.php?f=2&t=61086&p=408221&hilit=keys+map#p408221
    void parseKeyConfiguration(const std::string& keyConfigFile) {
        Ogre::ConfigFile cf;
        cf.load(keyConfigFile);
        Ogre::ConfigFile::SectionIterator secIt = cf.getSectionIterator();
        secIt.getNext();
        Ogre::String sectionstring = "";
        Ogre::String typestring = "";
        Ogre::String paramstring = "";
        while (secIt.hasMoreElements()) {
            sectionstring = secIt.peekNextKey();
            Ogre::ConfigFile::SettingsMultiMap* settings = secIt.getNext();
            //Do the code for section... e.g. print sectionstring
            for (Ogre::ConfigFile::SettingsMultiMap::iterator setIt = settings->begin(); setIt != settings->end(); ++setIt) {
                typestring = setIt->first;
                paramstring = setIt->second;
                // do the code for key/value
            }
        }
    }

    std::string getActionForKey(const OIS::KeyCode& keyCode) {
        return this->keyMapping.at(keyCode);
    }

};

#endif	/* KEYBINDINGS_H */

