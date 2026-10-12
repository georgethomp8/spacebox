/* ------------------------------------------------------------
name: "FaustSynth"
Code generated with Faust 2.23.4 (https://faust.grame.fr)
Compilation options: -lang cpp -scal -ftz 0
------------------------------------------------------------ */



#include <cmath>
#include <cstring>

/************************** BEGIN MapUI.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef FAUST_MAPUI_H
#define FAUST_MAPUI_H

#include <vector>
#include <map>
#include <string>

/************************** BEGIN UI.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2020 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __UI_H__
#define __UI_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

/*******************************************************************************
 * UI : Faust DSP User Interface
 * User Interface as expected by the buildUserInterface() method of a DSP.
 * This abstract class contains only the method that the Faust compiler can
 * generate to describe a DSP user interface.
 ******************************************************************************/

struct Soundfile;

template <typename REAL>
struct UIReal
{
    UIReal() {}
    virtual ~UIReal() {}
    
    // -- widget's layouts
    
    virtual void openTabBox(const char* label) = 0;
    virtual void openHorizontalBox(const char* label) = 0;
    virtual void openVerticalBox(const char* label) = 0;
    virtual void closeBox() = 0;
    
    // -- active widgets
    
    virtual void addButton(const char* label, REAL* zone) = 0;
    virtual void addCheckButton(const char* label, REAL* zone) = 0;
    virtual void addVerticalSlider(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    virtual void addHorizontalSlider(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    virtual void addNumEntry(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    
    // -- passive widgets
    
    virtual void addHorizontalBargraph(const char* label, REAL* zone, REAL min, REAL max) = 0;
    virtual void addVerticalBargraph(const char* label, REAL* zone, REAL min, REAL max) = 0;
    
    // -- soundfiles
    
    virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) = 0;
    
    // -- metadata declarations
    
    virtual void declare(REAL* zone, const char* key, const char* val) {}
};

struct UI : public UIReal<FAUSTFLOAT>
{
    UI() {}
    virtual ~UI() {}
};

#endif
/**************************  END  UI.h **************************/
/************************** BEGIN PathBuilder.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef FAUST_PATHBUILDER_H
#define FAUST_PATHBUILDER_H

#include <vector>
#include <string>
#include <algorithm>

/*******************************************************************************
 * PathBuilder : Faust User Interface
 * Helper class to build complete hierarchical path for UI items.
 ******************************************************************************/

class PathBuilder
{

    protected:
    
        std::vector<std::string> fControlsLevel;
       
    public:
    
        PathBuilder() {}
        virtual ~PathBuilder() {}
    
        std::string buildPath(const std::string& label) 
        {
            std::string res = "/";
            for (size_t i = 0; i < fControlsLevel.size(); i++) {
                res += fControlsLevel[i];
                res += "/";
            }
            res += label;
            std::replace(res.begin(), res.end(), ' ', '_');
            return res;
        }
    
        void pushLabel(const std::string& label) { fControlsLevel.push_back(label); }
        void popLabel() { fControlsLevel.pop_back(); }
    
};

#endif  // FAUST_PATHBUILDER_H
/**************************  END  PathBuilder.h **************************/

/*******************************************************************************
 * MapUI : Faust User Interface
 * This class creates a map of complete hierarchical path and zones for each UI items.
 ******************************************************************************/

class MapUI : public UI, public PathBuilder
{
    
    protected:
    
        // Complete path map
        std::map<std::string, FAUSTFLOAT*> fPathZoneMap;
    
        // Label zone map
        std::map<std::string, FAUSTFLOAT*> fLabelZoneMap;
    
    public:
        
        MapUI() {}
        virtual ~MapUI() {}
        
        // -- widget's layouts
        void openTabBox(const char* label)
        {
            pushLabel(label);
        }
        void openHorizontalBox(const char* label)
        {
            pushLabel(label);
        }
        void openVerticalBox(const char* label)
        {
            pushLabel(label);
        }
        void closeBox()
        {
            popLabel();
        }
        
        // -- active widgets
        void addButton(const char* label, FAUSTFLOAT* zone)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addCheckButton(const char* label, FAUSTFLOAT* zone)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        
        // -- passive widgets
        void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT fmin, FAUSTFLOAT fmax)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT fmin, FAUSTFLOAT fmax)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
    
        // -- soundfiles
        virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) {}
        
        // -- metadata declarations
        void declare(FAUSTFLOAT* zone, const char* key, const char* val)
        {}
        
        // set/get
        void setParamValue(const std::string& path, FAUSTFLOAT value)
        {
            if (fPathZoneMap.find(path) != fPathZoneMap.end()) {
                *fPathZoneMap[path] = value;
            } else if (fLabelZoneMap.find(path) != fLabelZoneMap.end()) {
                *fLabelZoneMap[path] = value;
            }
        }
        
        FAUSTFLOAT getParamValue(const std::string& path)
        {
            if (fPathZoneMap.find(path) != fPathZoneMap.end()) {
                return *fPathZoneMap[path];
            } else if (fLabelZoneMap.find(path) != fLabelZoneMap.end()) {
                return *fLabelZoneMap[path];
            } else {
                return FAUSTFLOAT(0);
            }
        }
    
        // map access 
        std::map<std::string, FAUSTFLOAT*>& getMap() { return fPathZoneMap; }
        
        int getParamsCount() { return int(fPathZoneMap.size()); }
        
        std::string getParamAddress(int index)
        { 
            std::map<std::string, FAUSTFLOAT*>::iterator it = fPathZoneMap.begin();
            while (index-- > 0 && it++ != fPathZoneMap.end()) {}
            return (*it).first;
        }
    
        std::string getParamAddress(FAUSTFLOAT* zone)
        {
            std::map<std::string, FAUSTFLOAT*>::iterator it = fPathZoneMap.begin();
            do {
                if ((*it).second == zone) return (*it).first;
            }
            while (it++ != fPathZoneMap.end());
            return "";
        }
    
        static bool endsWith(std::string const& str, std::string const& end)
        {
            size_t l1 = str.length();
            size_t l2 = end.length();
            return (l1 >= l2) && (0 == str.compare(l1 - l2, l2, end));
        }
};


#endif // FAUST_MAPUI_H
/**************************  END  MapUI.h **************************/
/************************** BEGIN meta.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __meta__
#define __meta__

struct Meta
{
    virtual ~Meta() {};
    virtual void declare(const char* key, const char* value) = 0;
    
};

#endif
/**************************  END  meta.h **************************/
/************************** BEGIN dsp.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __dsp__
#define __dsp__

#include <string>
#include <vector>

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

struct UI;
struct Meta;

/**
 * DSP memory manager.
 */

struct dsp_memory_manager {
    
    virtual ~dsp_memory_manager() {}
    
    virtual void* allocate(size_t size) = 0;
    virtual void destroy(void* ptr) = 0;
    
};

/**
* Signal processor definition.
*/

class dsp {

    public:

        dsp() {}
        virtual ~dsp() {}

        /* Return instance number of audio inputs */
        virtual int getNumInputs() = 0;
    
        /* Return instance number of audio outputs */
        virtual int getNumOutputs() = 0;
    
        /**
         * Trigger the ui_interface parameter with instance specific calls
         * to 'addBtton', 'addVerticalSlider'... in order to build the UI.
         *
         * @param ui_interface - the user interface builder
         */
        virtual void buildUserInterface(UI* ui_interface) = 0;
    
        /* Returns the sample rate currently used by the instance */
        virtual int getSampleRate() = 0;
    
        /**
         * Global init, calls the following methods:
         * - static class 'classInit': static tables initialization
         * - 'instanceInit': constants and instance state initialization
         *
         * @param sample_rate - the sampling rate in Hertz
         */
        virtual void init(int sample_rate) = 0;

        /**
         * Init instance state
         *
         * @param sample_rate - the sampling rate in Hertz
         */
        virtual void instanceInit(int sample_rate) = 0;

        /**
         * Init instance constant state
         *
         * @param sample_rate - the sampling rate in Hertz
         */
        virtual void instanceConstants(int sample_rate) = 0;
    
        /* Init default control parameters values */
        virtual void instanceResetUserInterface() = 0;
    
        /* Init instance state (delay lines...) */
        virtual void instanceClear() = 0;
 
        /**
         * Return a clone of the instance.
         *
         * @return a copy of the instance on success, otherwise a null pointer.
         */
        virtual dsp* clone() = 0;
    
        /**
         * Trigger the Meta* parameter with instance specific calls to 'declare' (key, value) metadata.
         *
         * @param m - the Meta* meta user
         */
        virtual void metadata(Meta* m) = 0;
    
        /**
         * DSP instance computation, to be called with successive in/out audio buffers.
         *
         * @param count - the number of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
         *
         */
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) = 0;
    
        /**
         * DSP instance computation: alternative method to be used by subclasses.
         *
         * @param date_usec - the timestamp in microsec given by audio driver.
         * @param count - the number of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (either float, double or quad)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (either float, double or quad)
         *
         */
        virtual void compute(double /*date_usec*/, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }
       
};

/**
 * Generic DSP decorator.
 */

class decorator_dsp : public dsp {

    protected:

        dsp* fDSP;

    public:

        decorator_dsp(dsp* dsp = nullptr):fDSP(dsp) {}
        virtual ~decorator_dsp() { delete fDSP; }

        virtual int getNumInputs() { return fDSP->getNumInputs(); }
        virtual int getNumOutputs() { return fDSP->getNumOutputs(); }
        virtual void buildUserInterface(UI* ui_interface) { fDSP->buildUserInterface(ui_interface); }
        virtual int getSampleRate() { return fDSP->getSampleRate(); }
        virtual void init(int sample_rate) { fDSP->init(sample_rate); }
        virtual void instanceInit(int sample_rate) { fDSP->instanceInit(sample_rate); }
        virtual void instanceConstants(int sample_rate) { fDSP->instanceConstants(sample_rate); }
        virtual void instanceResetUserInterface() { fDSP->instanceResetUserInterface(); }
        virtual void instanceClear() { fDSP->instanceClear(); }
        virtual decorator_dsp* clone() { return new decorator_dsp(fDSP->clone()); }
        virtual void metadata(Meta* m) { fDSP->metadata(m); }
        // Beware: subclasses usually have to overload the two 'compute' methods
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { fDSP->compute(count, inputs, outputs); }
        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { fDSP->compute(date_usec, count, inputs, outputs); }
    
};

/**
 * DSP factory class.
 */

class dsp_factory {
    
    protected:
    
        // So that to force sub-classes to use deleteDSPFactory(dsp_factory* factory);
        virtual ~dsp_factory() {}
    
    public:
    
        virtual std::string getName() = 0;
        virtual std::string getSHAKey() = 0;
        virtual std::string getDSPCode() = 0;
        virtual std::string getCompileOptions() = 0;
        virtual std::vector<std::string> getLibraryList() = 0;
        virtual std::vector<std::string> getIncludePathnames() = 0;
    
        virtual dsp* createDSPInstance() = 0;
    
        virtual void setMemoryManager(dsp_memory_manager* manager) = 0;
        virtual dsp_memory_manager* getMemoryManager() = 0;
    
};

/**
 * On Intel set FZ (Flush to Zero) and DAZ (Denormals Are Zero)
 * flags to avoid costly denormals.
 */

#ifdef __SSE__
    #include <xmmintrin.h>
    #ifdef __SSE2__
        #define AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8040)
    #else
        #define AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8000)
    #endif
#else
    #define AVOIDDENORMALS
#endif

#endif
/**************************  END  dsp.h **************************/

/* ------------------------------------------------------------
name: "Spacebox"
Code generated with Faust 2.81.10 (https://faust.grame.fr)
Compilation options: -lang cpp -ct 1 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __mydsp_H__
#define  __mydsp_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS
#define FAUSTCLASS mydsp
#endif

#ifdef __APPLE__
#define exp10f __exp10f
#define exp10 __exp10
#endif

#if defined(_WIN32)
#define RESTRICT __restrict
#else
#define RESTRICT __restrict__
#endif

static float mydsp_faustpower2_f(float value) {
    return value * value;
}

class mydsp : public dsp {
    
 private:
    
    FAUSTFLOAT fCheckbox0;
    int IOTA0;
    float fVec0[32768];
    int iVec1[2];
    int fSampleRate;
    float fConst0;
    float fConst1;
    float fConst2;
    FAUSTFLOAT fHslider0;
    float fRec0[2];
    float fConst3;
    FAUSTFLOAT fHslider1;
    float fRec2[2];
    float fConst4;
    float fRec1[2];
    FAUSTFLOAT fHslider2;
    float fRec3[2];
    float fVec2[2];
    float fVec3[32768];
    float fVec4[2];
    float fConst5;
    float fConst6;
    float fConst7;
    float fRec4[2];
    FAUSTFLOAT fEntry0;
    float fConst8;
    float fRec6[2];
    float fVec5[2];
    float fRec5[2];
    float fVec6[2];
    float fRec7[2];
    float fConst9;
    float fConst10;
    float fConst11;
    float fConst12;
    float fConst13;
    float fConst14;
    float fConst15;
    float fRec9[2];
    float fRec8[3];
    float fConst16;
    float fRec11[2];
    float fRec10[3];
    FAUSTFLOAT fHslider3;
    float fRec12[2];
    float fRec13[2];
    float fVec7[2];
    float fRec14[2];
    float fVec8[2];
    float fRec15[2];
    float fRec16[3];
    float fRec17[3];
    
 public:
    mydsp() {
    }
    
    void metadata(Meta* m) {
        m->declare("basics.lib/bypass2:author", "Julius Smith");
        m->declare("basics.lib/name", "Faust Basic Element Library");
        m->declare("basics.lib/version", "1.22.0");
        m->declare("compile_options", "-lang cpp -ct 1 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
        m->declare("delays.lib/name", "Faust Delay Library");
        m->declare("delays.lib/version", "1.2.0");
        m->declare("filename", "Spacebox.dsp");
        m->declare("filters.lib/fir:author", "Julius O. Smith III");
        m->declare("filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
        m->declare("filters.lib/fir:license", "MIT-style STK-4.3 license");
        m->declare("filters.lib/highpass:author", "Julius O. Smith III");
        m->declare("filters.lib/highpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
        m->declare("filters.lib/iir:author", "Julius O. Smith III");
        m->declare("filters.lib/iir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
        m->declare("filters.lib/iir:license", "MIT-style STK-4.3 license");
        m->declare("filters.lib/lowpass0_highpass1", "MIT-style STK-4.3 license");
        m->declare("filters.lib/lowpass0_highpass1:author", "Julius O. Smith III");
        m->declare("filters.lib/lowpass:author", "Julius O. Smith III");
        m->declare("filters.lib/lowpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
        m->declare("filters.lib/lowpass:license", "MIT-style STK-4.3 license");
        m->declare("filters.lib/name", "Faust Filters Library");
        m->declare("filters.lib/tf1:author", "Julius O. Smith III");
        m->declare("filters.lib/tf1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
        m->declare("filters.lib/tf1:license", "MIT-style STK-4.3 license");
        m->declare("filters.lib/tf1s:author", "Julius O. Smith III");
        m->declare("filters.lib/tf1s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
        m->declare("filters.lib/tf1s:license", "MIT-style STK-4.3 license");
        m->declare("filters.lib/tf2:author", "Julius O. Smith III");
        m->declare("filters.lib/tf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
        m->declare("filters.lib/tf2:license", "MIT-style STK-4.3 license");
        m->declare("filters.lib/tf2s:author", "Julius O. Smith III");
        m->declare("filters.lib/tf2s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
        m->declare("filters.lib/tf2s:license", "MIT-style STK-4.3 license");
        m->declare("filters.lib/version", "1.7.1");
        m->declare("maths.lib/author", "GRAME");
        m->declare("maths.lib/copyright", "GRAME");
        m->declare("maths.lib/license", "LGPL with exception");
        m->declare("maths.lib/name", "Faust Math Library");
        m->declare("maths.lib/version", "2.9.0");
        m->declare("misceffects.lib/dryWetMixer:author", "David Braun, revised by Stéphane Letz");
        m->declare("misceffects.lib/name", "Misc Effects Library");
        m->declare("misceffects.lib/version", "2.5.1");
        m->declare("name", "Spacebox");
        m->declare("oscillators.lib/lf_sawpos:author", "Bart Brouns, revised by Stéphane Letz");
        m->declare("oscillators.lib/lf_sawpos:licence", "STK-4.3");
        m->declare("oscillators.lib/lf_triangle:author", "Bart Brouns");
        m->declare("oscillators.lib/lf_triangle:licence", "STK-4.3");
        m->declare("oscillators.lib/name", "Faust Oscillator Library");
        m->declare("oscillators.lib/saw1:author", "Bart Brouns");
        m->declare("oscillators.lib/saw1:licence", "STK-4.3");
        m->declare("oscillators.lib/version", "1.6.0");
        m->declare("platform.lib/name", "Generic Platform Library");
        m->declare("platform.lib/version", "1.3.0");
        m->declare("routes.lib/name", "Faust Signal Routing Library");
        m->declare("routes.lib/version", "1.2.0");
        m->declare("signals.lib/name", "Faust Signal Routing Library");
        m->declare("signals.lib/version", "1.6.0");
    }

    virtual int getNumInputs() {
        return 2;
    }
    virtual int getNumOutputs() {
        return 2;
    }
    
    static void classInit(int sample_rate) {
    }
    
    virtual void instanceConstants(int sample_rate) {
        fSampleRate = sample_rate;
        fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, static_cast<float>(fSampleRate)));
        fConst1 = 44.1f / fConst0;
        fConst2 = 1.0f - fConst1;
        fConst3 = 0.1f * fConst0 + 1.0f;
        fConst4 = 0.5f / fConst0;
        fConst5 = 1.0f / std::tan(251.32741f / fConst0);
        fConst6 = 1.0f - fConst5;
        fConst7 = 1.0f / (fConst5 + 1.0f);
        fConst8 = 0.25f / fConst0;
        fConst9 = std::tan(31415.926f / fConst0);
        fConst10 = 2.0f * (1.0f - 1.0f / mydsp_faustpower2_f(fConst9));
        fConst11 = 1.0f / fConst9;
        fConst12 = (fConst11 + -1.4142135f) / fConst9 + 1.0f;
        fConst13 = 1.0f / ((fConst11 + 1.4142135f) / fConst9 + 1.0f);
        fConst14 = 0.0214f * fConst0 + 11.0f;
        fConst15 = 0.863f / fConst0;
        fConst16 = 0.513f / fConst0;
    }
    
    virtual void instanceResetUserInterface() {
        fCheckbox0 = static_cast<FAUSTFLOAT>(0.0f);
        fHslider0 = static_cast<FAUSTFLOAT>(0.5f);
        fHslider1 = static_cast<FAUSTFLOAT>(1.0f);
        fHslider2 = static_cast<FAUSTFLOAT>(1.0f);
        fEntry0 = static_cast<FAUSTFLOAT>(1.0f);
        fHslider3 = static_cast<FAUSTFLOAT>(0.0f);
    }
    
    virtual void instanceClear() {
        IOTA0 = 0;
        for (int l0 = 0; l0 < 32768; l0 = l0 + 1) {
            fVec0[l0] = 0.0f;
        }
        for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
            iVec1[l1] = 0;
        }
        for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
            fRec0[l2] = 0.0f;
        }
        for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
            fRec2[l3] = 0.0f;
        }
        for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
            fRec1[l4] = 0.0f;
        }
        for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
            fRec3[l5] = 0.0f;
        }
        for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
            fVec2[l6] = 0.0f;
        }
        for (int l7 = 0; l7 < 32768; l7 = l7 + 1) {
            fVec3[l7] = 0.0f;
        }
        for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
            fVec4[l8] = 0.0f;
        }
        for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
            fRec4[l9] = 0.0f;
        }
        for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
            fRec6[l10] = 0.0f;
        }
        for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
            fVec5[l11] = 0.0f;
        }
        for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
            fRec5[l12] = 0.0f;
        }
        for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
            fVec6[l13] = 0.0f;
        }
        for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
            fRec7[l14] = 0.0f;
        }
        for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
            fRec9[l15] = 0.0f;
        }
        for (int l16 = 0; l16 < 3; l16 = l16 + 1) {
            fRec8[l16] = 0.0f;
        }
        for (int l17 = 0; l17 < 2; l17 = l17 + 1) {
            fRec11[l17] = 0.0f;
        }
        for (int l18 = 0; l18 < 3; l18 = l18 + 1) {
            fRec10[l18] = 0.0f;
        }
        for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
            fRec12[l19] = 0.0f;
        }
        for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
            fRec13[l20] = 0.0f;
        }
        for (int l21 = 0; l21 < 2; l21 = l21 + 1) {
            fVec7[l21] = 0.0f;
        }
        for (int l22 = 0; l22 < 2; l22 = l22 + 1) {
            fRec14[l22] = 0.0f;
        }
        for (int l23 = 0; l23 < 2; l23 = l23 + 1) {
            fVec8[l23] = 0.0f;
        }
        for (int l24 = 0; l24 < 2; l24 = l24 + 1) {
            fRec15[l24] = 0.0f;
        }
        for (int l25 = 0; l25 < 3; l25 = l25 + 1) {
            fRec16[l25] = 0.0f;
        }
        for (int l26 = 0; l26 < 3; l26 = l26 + 1) {
            fRec17[l26] = 0.0f;
        }
    }
    
    virtual void init(int sample_rate) {
        classInit(sample_rate);
        instanceInit(sample_rate);
    }
    
    virtual void instanceInit(int sample_rate) {
        instanceConstants(sample_rate);
        instanceResetUserInterface();
        instanceClear();
    }
    
    virtual mydsp* clone() {
        return new mydsp();
    }
    
    virtual int getSampleRate() {
        return fSampleRate;
    }
    
    virtual void buildUserInterface(UI* ui_interface) {
        ui_interface->openVerticalBox("Spacebox");
        ui_interface->addCheckButton("bypass", &fCheckbox0);
        ui_interface->addHorizontalSlider("lfo_depth_scale", &fHslider2, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(4.0f), FAUSTFLOAT(0.01f));
        ui_interface->addHorizontalSlider("lfo_rate_scale", &fHslider1, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(4.0f), FAUSTFLOAT(0.01f));
        ui_interface->addHorizontalSlider("mix", &fHslider0, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
        ui_interface->declare(&fEntry0, "style", "menu{'Juno 60 I':1;'Juno 60 II':2,'Dimension D 1':3,'Dimension D 2':4,'Dimension D 3':5,'Dimension D 4':6}");
        ui_interface->addNumEntry("mode", &fEntry0, FAUSTFLOAT(1.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(6.0f), FAUSTFLOAT(1.0f));
        ui_interface->declare(&fHslider3, "unit", "dB");
        ui_interface->addHorizontalSlider("output_gain", &fHslider3, FAUSTFLOAT(0.0f), FAUSTFLOAT(-24.0f), FAUSTFLOAT(24.0f), FAUSTFLOAT(0.1f));
        ui_interface->closeBox();
    }
    
    virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
        FAUSTFLOAT* input0 = inputs[0];
        FAUSTFLOAT* input1 = inputs[1];
        FAUSTFLOAT* output0 = outputs[0];
        FAUSTFLOAT* output1 = outputs[1];
        int iSlow0 = static_cast<int>(static_cast<float>(fCheckbox0));
        float fSlow1 = fConst1 * static_cast<float>(fHslider0);
        float fSlow2 = fConst1 * static_cast<float>(fHslider1);
        float fSlow3 = fConst1 * static_cast<float>(fHslider2);
        float fSlow4 = static_cast<float>(fEntry0) + -1.0f;
        int iSlow5 = fSlow4 >= 5.0f;
        float fSlow6 = static_cast<float>(iSlow5);
        int iSlow7 = fSlow4 >= 4.0f;
        float fSlow8 = static_cast<float>(iSlow7);
        float fSlow9 = static_cast<float>(1 - iSlow7);
        float fSlow10 = static_cast<float>(1 - iSlow5);
        int iSlow11 = fSlow4 >= 3.0f;
        float fSlow12 = static_cast<float>(iSlow11);
        int iSlow13 = fSlow4 >= 2.0f;
        float fSlow14 = static_cast<float>(iSlow13);
        int iSlow15 = fSlow4 >= 1.0f;
        float fSlow16 = static_cast<float>(iSlow15);
        float fSlow17 = static_cast<float>(1 - iSlow15);
        float fSlow18 = fConst13 * static_cast<float>(1 - iSlow13);
        float fSlow19 = static_cast<float>(1 - iSlow11);
        float fSlow20 = fConst1 * std::pow(1e+01f, 0.05f * static_cast<float>(fHslider3));
        for (int i0 = 0; i0 < count; i0 = i0 + 1) {
            float fTemp0 = static_cast<float>(input0[i0]);
            float fTemp1 = ((iSlow0) ? 0.0f : fTemp0);
            fVec0[IOTA0 & 32767] = fTemp1;
            iVec1[0] = 1;
            fRec0[0] = fSlow1 + fConst2 * fRec0[1];
            float fTemp2 = 1.0f - fRec0[0];
            int iTemp3 = 1 - iVec1[1];
            fRec2[0] = fSlow2 + fConst2 * fRec2[1];
            float fTemp4 = ((iTemp3) ? 0.0f : fRec1[1] + fConst4 * fRec2[0]);
            fRec1[0] = fTemp4 - std::floor(fTemp4);
            float fTemp5 = 1.0f - std::fabs(2.0f * fRec1[0] + -1.0f);
            fRec3[0] = fSlow3 + fConst2 * fRec3[1];
            float fTemp6 = fConst0 * (0.003f * fRec3[0] * fTemp5 + 0.0075f);
            int iTemp7 = static_cast<int>(fTemp6);
            float fTemp8 = std::floor(fTemp6);
            float fTemp9 = (fTemp6 - fTemp8) * fVec0[(IOTA0 - static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iTemp7 + 1))))) & 32767];
            float fTemp10 = fVec0[(IOTA0 - static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iTemp7))))) & 32767] * (fTemp8 + (1.0f - fTemp6));
            float fTemp11 = fTemp10 + fTemp9;
            fVec2[0] = fTemp11;
            float fTemp12 = static_cast<float>(input1[i0]);
            float fTemp13 = ((iSlow0) ? 0.0f : fTemp12);
            fVec3[IOTA0 & 32767] = fTemp13;
            float fTemp14 = fConst0 * (0.0015f * fRec3[0] * (1.0f - (2.0f * fTemp5 + -1.0f)) + 0.0075f);
            int iTemp15 = static_cast<int>(fTemp14);
            float fTemp16 = std::floor(fTemp14);
            float fTemp17 = (fTemp14 - fTemp16) * fVec3[(IOTA0 - static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iTemp15 + 1))))) & 32767];
            float fTemp18 = fVec3[(IOTA0 - static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iTemp15))))) & 32767] * (fTemp16 + (1.0f - fTemp14));
            float fTemp19 = fTemp18 + fTemp17;
            fVec4[0] = fTemp19;
            fRec4[0] = -(fConst7 * (fConst6 * fRec4[1] + fConst5 * (fTemp19 - fVec4[1])));
            float fTemp20 = fRec4[0] + fTemp1;
            float fTemp21 = ((iTemp3) ? 0.0f : fRec6[1] + fConst8 * fRec2[0]);
            fRec6[0] = fTemp21 - std::floor(fTemp21);
            float fTemp22 = 1.0f - std::fabs(2.0f * fRec6[0] + -1.0f);
            float fTemp23 = fRec3[0] * (1.0f - (2.0f * fTemp22 + -1.0f));
            float fTemp24 = fConst0 * (0.0025f * fTemp23 + 0.0075f);
            int iTemp25 = static_cast<int>(fTemp24);
            float fTemp26 = std::floor(fTemp24);
            float fTemp27 = (fTemp24 - fTemp26) * fVec3[(IOTA0 - static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iTemp25 + 1))))) & 32767];
            float fTemp28 = fVec3[(IOTA0 - static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iTemp25))))) & 32767] * (fTemp26 + (1.0f - fTemp24));
            float fTemp29 = fTemp28 + fTemp27;
            fVec5[0] = fTemp29;
            fRec5[0] = -(fConst7 * (fConst6 * fRec5[1] + fConst5 * (fTemp29 - fVec5[1])));
            float fTemp30 = fRec3[0] * fTemp22;
            float fTemp31 = fConst0 * (0.005f * fTemp30 + 0.0075f);
            float fTemp32 = std::floor(fTemp31);
            int iTemp33 = static_cast<int>(fTemp31);
            float fTemp34 = fVec0[(IOTA0 - static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iTemp33))))) & 32767] * (fTemp32 + (1.0f - fTemp31));
            float fTemp35 = (fTemp31 - fTemp32) * fVec0[(IOTA0 - static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iTemp33 + 1))))) & 32767];
            float fTemp36 = fConst0 * (0.002f * fTemp23 + 0.01f);
            int iTemp37 = static_cast<int>(fTemp36);
            float fTemp38 = std::floor(fTemp36);
            float fTemp39 = (fTemp36 - fTemp38) * fVec3[(IOTA0 - static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iTemp37 + 1))))) & 32767];
            float fTemp40 = fVec3[(IOTA0 - static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iTemp37))))) & 32767] * (fTemp38 + (1.0f - fTemp36));
            float fTemp41 = fTemp40 + fTemp39;
            fVec6[0] = fTemp41;
            fRec7[0] = -(fConst7 * (fConst6 * fRec7[1] + fConst5 * (fTemp41 - fVec6[1])));
            float fTemp42 = fConst0 * (0.004f * fTemp30 + 0.01f);
            float fTemp43 = std::floor(fTemp42);
            int iTemp44 = static_cast<int>(fTemp42);
            float fTemp45 = fVec0[(IOTA0 - static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iTemp44))))) & 32767] * (fTemp43 + (1.0f - fTemp42));
            float fTemp46 = (fTemp42 - fTemp43) * fVec0[(IOTA0 - static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iTemp44 + 1))))) & 32767];
            float fTemp47 = ((iTemp3) ? 0.0f : fRec9[1] + fConst15 * fRec2[0]);
            fRec9[0] = fTemp47 - std::floor(fTemp47);
            float fTemp48 = 0.001845f * fRec3[0] * (2.0f * (1.0f - std::fabs(2.0f * fRec9[0] + -1.0f)) + -1.0f);
            float fTemp49 = fConst0 * (0.003505f - fTemp48);
            int iTemp50 = static_cast<int>(fTemp49);
            float fTemp51 = std::floor(fTemp49);
            fRec8[0] = fVec3[(IOTA0 - static_cast<int>(std::min<float>(fConst14, static_cast<float>(std::max<int>(0, iTemp50))))) & 32767] * (fTemp51 + (1.0f - fTemp49)) + (fTemp49 - fTemp51) * fVec3[(IOTA0 - static_cast<int>(std::min<float>(fConst14, static_cast<float>(std::max<int>(0, iTemp50 + 1))))) & 32767] - fConst13 * (fConst12 * fRec8[2] + fConst10 * fRec8[1]);
            float fTemp52 = ((iTemp3) ? 0.0f : fRec11[1] + fConst16 * fRec2[0]);
            fRec11[0] = fTemp52 - std::floor(fTemp52);
            float fTemp53 = 0.001845f * fRec3[0] * (2.0f * (1.0f - std::fabs(2.0f * fRec11[0] + -1.0f)) + -1.0f);
            float fTemp54 = fConst0 * (0.003505f - fTemp53);
            int iTemp55 = static_cast<int>(fTemp54);
            float fTemp56 = std::floor(fTemp54);
            fRec10[0] = fVec3[(IOTA0 - static_cast<int>(std::min<float>(fConst14, static_cast<float>(std::max<int>(0, iTemp55))))) & 32767] * (fTemp56 + (1.0f - fTemp54)) + (fTemp54 - fTemp56) * fVec3[(IOTA0 - static_cast<int>(std::min<float>(fConst14, static_cast<float>(std::max<int>(0, iTemp55 + 1))))) & 32767] - fConst13 * (fConst12 * fRec10[2] + fConst10 * fRec10[1]);
            fRec12[0] = fSlow20 + fConst2 * fRec12[1];
            output0[i0] = static_cast<FAUSTFLOAT>(fRec12[0] * ((iSlow0) ? fTemp0 : fRec0[0] * (fSlow19 * (fSlow18 * (fSlow17 * (fRec10[2] + fRec10[0] + 2.0f * fRec10[1]) + fSlow16 * (fRec8[2] + fRec8[0] + 2.0f * fRec8[1])) + fSlow14 * (fTemp46 + fTemp45 + fRec7[0] + fTemp1)) + fSlow12 * (fSlow10 * (fSlow9 * (fTemp35 + fTemp34 + fRec5[0] + fTemp1) + fSlow8 * (fTemp9 + fTemp10 + fTemp20)) + fSlow6 * (fTemp20 + 0.31622776f * fTemp11))) + fTemp2 * fTemp1));
            fRec13[0] = -(fConst7 * (fConst6 * fRec13[1] + fConst5 * (fTemp11 - fVec2[1])));
            float fTemp57 = fRec13[0] + fTemp13;
            float fTemp58 = fTemp34 + fTemp35;
            fVec7[0] = fTemp58;
            fRec14[0] = -(fConst7 * (fConst6 * fRec14[1] + fConst5 * (fTemp58 - fVec7[1])));
            float fTemp59 = fTemp45 + fTemp46;
            fVec8[0] = fTemp59;
            fRec15[0] = -(fConst7 * (fConst6 * fRec15[1] + fConst5 * (fTemp59 - fVec8[1])));
            float fTemp60 = fConst0 * (fTemp48 + 0.003505f);
            int iTemp61 = static_cast<int>(fTemp60);
            float fTemp62 = std::floor(fTemp60);
            fRec16[0] = fVec0[(IOTA0 - static_cast<int>(std::min<float>(fConst14, static_cast<float>(std::max<int>(0, iTemp61))))) & 32767] * (fTemp62 + (1.0f - fTemp60)) + (fTemp60 - fTemp62) * fVec0[(IOTA0 - static_cast<int>(std::min<float>(fConst14, static_cast<float>(std::max<int>(0, iTemp61 + 1))))) & 32767] - fConst13 * (fConst12 * fRec16[2] + fConst10 * fRec16[1]);
            float fTemp63 = fConst0 * (fTemp53 + 0.003505f);
            int iTemp64 = static_cast<int>(fTemp63);
            float fTemp65 = std::floor(fTemp63);
            fRec17[0] = fVec0[(IOTA0 - static_cast<int>(std::min<float>(fConst14, static_cast<float>(std::max<int>(0, iTemp64))))) & 32767] * (fTemp65 + (1.0f - fTemp63)) + (fTemp63 - fTemp65) * fVec0[(IOTA0 - static_cast<int>(std::min<float>(fConst14, static_cast<float>(std::max<int>(0, iTemp64 + 1))))) & 32767] - fConst13 * (fConst12 * fRec17[2] + fConst10 * fRec17[1]);
            output1[i0] = static_cast<FAUSTFLOAT>(fRec12[0] * ((iSlow0) ? fTemp12 : fRec0[0] * (fSlow19 * (fSlow18 * (fSlow17 * (fRec17[2] + fRec17[0] + 2.0f * fRec17[1]) + fSlow16 * (fRec16[2] + fRec16[0] + 2.0f * fRec16[1])) + fSlow14 * (fTemp39 + fTemp40 + fRec15[0] + fTemp13)) + fSlow12 * (fSlow10 * (fSlow9 * (fTemp27 + fTemp28 + fRec14[0] + fTemp13) + fSlow8 * (fTemp17 + fTemp18 + fTemp57)) + fSlow6 * (fTemp57 + 0.31622776f * fTemp19))) + fTemp2 * fTemp13));
            IOTA0 = IOTA0 + 1;
            iVec1[1] = iVec1[0];
            fRec0[1] = fRec0[0];
            fRec2[1] = fRec2[0];
            fRec1[1] = fRec1[0];
            fRec3[1] = fRec3[0];
            fVec2[1] = fVec2[0];
            fVec4[1] = fVec4[0];
            fRec4[1] = fRec4[0];
            fRec6[1] = fRec6[0];
            fVec5[1] = fVec5[0];
            fRec5[1] = fRec5[0];
            fVec6[1] = fVec6[0];
            fRec7[1] = fRec7[0];
            fRec9[1] = fRec9[0];
            fRec8[2] = fRec8[1];
            fRec8[1] = fRec8[0];
            fRec11[1] = fRec11[0];
            fRec10[2] = fRec10[1];
            fRec10[1] = fRec10[0];
            fRec12[1] = fRec12[0];
            fRec13[1] = fRec13[0];
            fVec7[1] = fVec7[0];
            fRec14[1] = fRec14[0];
            fVec8[1] = fVec8[0];
            fRec15[1] = fRec15[0];
            fRec16[2] = fRec16[1];
            fRec16[1] = fRec16[0];
            fRec17[2] = fRec17[1];
            fRec17[1] = fRec17[0];
        }
    }

};

#endif


