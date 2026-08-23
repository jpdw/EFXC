#ifndef PROFILE_H_INCLUDED
#define PROFILE_H_INCLUDED


enum ProfileType { SWITCHER, RGB, OTHER };

class Profile {
    public:
        Profile();
        bool loadProfile();                             /* loads profile and sets up globabl objects */
        const char* getName();                          /* Profile name */
        void setName(char* name);                       /* Set the pr*/
        bool profileNameIs(const char* name);

    private:
        char* _profile_name_str;
        ProfileType _profile_type;
};



extern void setup_config();
extern void loop_config();

#endif