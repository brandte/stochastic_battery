//
// Generated file, do not edit! Created by nedtool 5.0 from power_update.msg.
//

#ifndef __POWER_UPDATE_M_H
#define __POWER_UPDATE_M_H

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0500
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



/**
 * Class generated from <tt>power_update.msg:1</tt> by nedtool.
 * <pre>
 * message power_update
 * {
 *     int power_consum;
 *     char current_activity;
 * }
 * </pre>
 */
class power_update : public ::omnetpp::cMessage
{
  protected:
    int power_consum;
    char current_activity;

  private:
    void copy(const power_update& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const power_update&);

  public:
    power_update(const char *name=nullptr, int kind=0);
    power_update(const power_update& other);
    virtual ~power_update();
    power_update& operator=(const power_update& other);
    virtual power_update *dup() const {return new power_update(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b);

    // field getter/setter methods
    virtual int getPower_consum() const;
    virtual void setPower_consum(int power_consum);
    virtual char getCurrent_activity() const;
    virtual void setCurrent_activity(char current_activity);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const power_update& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, power_update& obj) {obj.parsimUnpack(b);}


#endif // ifndef __POWER_UPDATE_M_H

