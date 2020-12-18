/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef IDATAROUTER_H
#define IDATAROUTER_H

class IDataRouter
{
public:
    virtual ~IDataRouter() {}

    // Poll interval is in milliseconds
    virtual double GetPollInterval() const = 0;
    virtual void SetPollInterval(double value) = 0;
    virtual bool StartThread() = 0;
    virtual bool StopThread() = 0;
};

#endif // IDATAROUTER_H
