#ifndef SKILL_HPP_
#define SKILL_HPP_

class Skill {
    public:
        float xp;
        
        Skill();
        Skill(float xp);
        Skill(const Skill& other);
        int operator () () const;
        bool increase(float value);
};

#endif
