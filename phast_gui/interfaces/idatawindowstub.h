/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef IDATAWINDOWSTUB_H
#define IDATAWINDOWSTUB_H

class IDataWindow;
class IDataPlot;
class IBinningClass;

class IDataWindowStub
{
public:
    virtual ~IDataWindowStub() {}

    virtual IDataWindow* GetDataWindow() = 0;
    virtual IDataPlot* GetDataPlot() = 0;
    virtual IBinningClass* GetBinningClass() = 0;
};

#endif // IDATAWINDOWSTUB_H
