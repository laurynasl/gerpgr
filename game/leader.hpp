#ifndef LEADER_HPP_
#define LEADER_HPP_

/** \brief Faction leader
 * 
 * His attributes may be: age, maximum age.
 * There is a difference if a leader is charismatic or not. He can cause wars, make peace depending on what he likes, 
 * Agressive, builder, technologist, militarist, defender
 */
class Leader {
    protected:
    public:
        Leader();
        Leader(const Leader& other);
        ~Leader();
};

#endif
