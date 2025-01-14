/*
 * ATLASPix3_SoftAndFirmware
 * Copyright (C) 2019  Rudolf Schimassek (rudolf.schimassek@kit.edu)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "trimming.h"

SCurve::SCurve() :  points(std::vector<std::pair<double, unsigned int> >()), maximum(0),
            threshold(-1), noise(-1),
            windowstart(-1), windowend(-1), windownoise(-1)
{

}

void SCurve::SetMaximum(int max)
{
    if(max >= 0)
        maximum = max;
}

int SCurve::GetMaximum()
{
    return maximum;
}

void SCurve::AddPoint(double voltage, int hits)
{
    //if(hits > maximum)
    //    maximum = hits;
    points.push_back(std::make_pair(voltage, hits));
}

int SCurve::AddHits(double voltage, int hits)
{
    bool done = false;
    int entries = 0;
    for(auto& it : points)
    {
        if(std::abs(it.first - voltage) < 1e-5)
        {
            done = true;
            it.second += static_cast<unsigned int>(hits);
            entries = int(it.second);
        }
    }
    if(!done)
    {
        AddPoint(voltage, hits);
        entries = hits;
    }

    return entries;
}

void SCurve::ClearPoints()
{
    maximum = 0;
    threshold = -1;
    noise = -1;

    points.clear();
}

unsigned int SCurve::GetNumPoints()
{
    return points.size();
}

std::pair<double, unsigned int> SCurve::GetPoint(unsigned int index)
{
    if(index < points.size())
        return points[index];
    else
        return std::make_pair(double(-1e10), static_cast<unsigned int>(0));
}

std::pair<double, double> SCurve::GetNormedPoint(unsigned int index)
{
    if(index < points.size())
    {
        std::pair<double, double> pt;
        pt.first  = points[index].first;
        pt.second = points[index].second/double(maximum);
        return pt;
    }
    else
        return std::make_pair(double(-1e10), double(0));
}

double SCurve::GetPointVoltage(unsigned int index)
{
    if(index < points.size())
        return points[index].first;
    else
        return -1e10;
}

unsigned int SCurve::GetPointSignals(unsigned int index)
{
    if(index < points.size())
        return points[index].second;
    else
        return 0;
}

int SCurve::GetSignalsToVolt(double voltage)
{
    for(auto& it : points)
        if(std::abs(it.first - voltage) < 0.001)
            return int(it.second);

    return -1;
}

double SCurve::GetPointNormedSignals(unsigned int index)
{
    if(index < points.size())
        return points[index].second / double(maximum);
    else
        return 0;
}

int SCurve::GetIntegral()
{
    int integral = 0;
    for(auto& it : points)
        integral += it.second;

    return integral;
}

double SCurve::GetThreshold()
{
    return threshold;
}

void SCurve::SetThreshold(double thr)
{
    threshold = thr;
}

double SCurve::GetNoise()
{
    return noise;
}

void SCurve::SetNoise(double noise)
{
    this->noise = noise;
}

double SCurve::GetWindowStart()
{
    return windowstart;
}

void SCurve::SetWindowStart(double start)
{
    windowstart = start;
}

double SCurve::GetWindowEnd()
{
    return windowend;
}

void SCurve::SetWindowEnd(double end)
{
    windowend = end;
}

double SCurve::GetWindowNoise()
{
    return windownoise;
}

void SCurve::SetWindowNoise(double noise)
{
    windownoise = noise;
}

std::map<double, double> SCurve::GetNormedSortedMap()
{
    std::map<double, double> data;

    for(auto& it : points)
        data.insert(std::make_pair(it.first,it.second/double(maximum)));

    return data;
}

std::string SCurve::GenerateString(std::string title, bool normed)
{
    std::stringstream s("");

    //header lines:
    s << "# SCurve: " << title << std::endl;
    if(std::abs(threshold - (-1)) > 1e-5 || std::abs(noise - (-1)) > 1e-5)
        s << "# Threshold: " << threshold << std::endl
          << "# Noise:     " << noise << std::endl;
    else if(std::abs(windowstart - (-1)) > 1e-5 || std::abs(windowend - (-1)) > 1e-5 || std::abs(windownoise - (-1)) > 1e-5)
        s << "# Window Start: " << windowstart << std::endl
          << "# Window End:   " << windowend << std::endl
          << "# Noise:        " << windownoise << std::endl;

    //data lines (including legend):
    if(normed)
    {
        s << "# inj. Voltage (in V); Fraction of Signals detected" << std::endl;
        for(auto& it : points)
            s << it.first << "\t" << it.second/double(maximum) << std::endl;
    }
    else
    {
        s << "# inj. Voltage (in V); Number of Signals detected (of " << maximum << ")" << std::endl;
        for(auto& it : points)
            s << it.first << "\t" << it.second << std::endl;
    }

    return s.str();
}

double SCurve::EvalSCurve(double x, double x0, double width)
{
    return (1. + erf((x-x0) / sqrt(2) / width)) * 0.5; // / 2.;
}

double SCurve::EvalWindow(double x, double x0, double x1, double width)
{
    return (1. + erf((x - x0) / sqrt(2) / width)) * (1. + erf((x1 - x) / sqrt(2) / width)) * 0.25;
}

bool smaller(std::pair<double, unsigned int> lhs, std::pair<double, unsigned int> rhs)
{
    return lhs.first < rhs.first;
}

void SCurve::Sort()
{
    std::sort(points.begin(), points.end(), smaller);
}


Trimming::Trimming(const Configuration &config) : interval_start(0), interval_step(1),
    interval_end(255), numsignals(100), maxsimultaneouspixels(4), usetrigID(false),//ivan change maxsimultaneouspixels from 20 to 4
    maxtimeperpoint(10000)
{
    this->config = config;
}

Configuration* Trimming::GetConfig()
{
    return &config;
}

void Trimming::SetConfig(const Configuration &config)
{
    this->config = config;
}

int Trimming::GetIntervalStart() const
{
    return interval_start;
}

void Trimming::SetIntervalStart(int value)
{
    interval_start = value;
}

int Trimming::GetIntervalEnd() const
{
    return interval_end;
}

void Trimming::SetIntervalEnd(int value)
{
    interval_end = value;
}

int Trimming::GetIntervalStep() const
{
    return interval_step;
}

void Trimming::SetIntervalStep(int value)
{
    interval_step = value;
}

int Trimming::GetNumSignals() const
{
    return numsignals;
}

void Trimming::SetNumSignals(int value)
{
    numsignals = value;
}

int Trimming::GetMaxSimultaneousPixels() const
{
    return maxsimultaneouspixels;
}

void Trimming::SetMaxSimultaneousPixels(int value)
{
    if(value > 0)
        maxsimultaneouspixels = value;
}

bool Trimming::GetTriggerIDUsage() const
{
    return usetrigID;
}

void Trimming::SetTriggerIDUsage(bool usetriggerid)
{
    this->usetrigID = usetriggerid;
}

int Trimming::GetMaxTime() const
{
    return maxtimeperpoint;
}

void Trimming::SetMaxTime(int value)
{
    if(value > 0)
        maxtimeperpoint = value;
}

void Trimming::ConfigureInjections(int row, bool flush, int startcol, int endcol)
{
    if(row < 0 || row >= AP3rows)
        return;


    ASIC_Config2* rowconfig = config.GetATLASPixConfig(Configuration::row);
    ASIC_Config2* colconfig = config.GetATLASPixConfig(Configuration::column);

    //injections only to passed row:
    for(int i = 0; i < AP3rows; ++i)
    {
        std::stringstream sr("");
        sr << "eninj_row_" << i;

        rowconfig->SetParameter(sr.str(), (i == row)?1:0);
    }

    //injection to all pixels in the row:
    for(int i = 0; i < AP3columns; ++i)
    {
        std::stringstream sc("");
        sc << "en_inject_col_" << i;

        if(i >= startcol && i <= endcol)
            colconfig->SetParameter(sc.str(), 1);
        else
            colconfig->SetParameter(sc.str(), 0);
    }

    if(flush){
        config.SendUpdate(Configuration::colrow);
        config.SendUpdate(Configuration::dac | Configuration::vdac);//Ivan
        config.WriteRAMRow(row);//ivan
    }
}

std::map<Dataset, SCurve> Trimming::MeasureFastSCurve(bool *running, int numactivepixels, int sleep)
{
    if(running == nullptr || *running == false)
        return std::map<Dataset, SCurve>();

    //backup injection settings:
    //ivan commented



    Injection_Config* injconf_backup = config.GetInjectionConfig();
    if(injconf_backup == nullptr)
    {
        std::cerr << "Error: Injection config empty, aborting SCurve." << std::endl;
        return std::map<Dataset,SCurve>();
    }
    Injection_Config injconf = *injconf_backup;
    config.SetInjectionConfig(&injconf);
    injconf.SetNumPulseSets(static_cast<unsigned int>(numsignals));



    //backup VDAC settings:
    ASIC_Config2* vdac_backup = config.GetATLASPixConfig(Configuration::vdac);
    if(vdac_backup == nullptr)
    {
        std::cerr << "Error: VDAC config empty, aborting SCurve" << std::endl;
        return std::map<Dataset, SCurve>();
    }
    ASIC_Config2 vdac = *vdac_backup;
    config.SetATLASPixConfig(Configuration::vdac, &vdac);

    //SCurve container:
    std::map<Dataset, SCurve> scurves;

    injconf.SetNumPulseSets(uint(numsignals));

    config.StopInjections(false);

    //ivan commented
    config.ConfigureInjections(true);

    //reset the trigger ID:
    NexysIO* nexys = config.GetNexysIO();
    nexys->Write(43, 1, false, 8);
    nexys->Write(43, 0, false, 1);
    config.ResetFastReadout(-1, true);

    int stepcounter = 0;
    const int maxsteps = 20 / numactivepixels;
    int readsize = numactivepixels;
    if(readsize > 5)
        readsize = 5;
    if(readsize < 1)
        readsize = 1;
    bool skipped = false;

    std::vector<Dataset> data;

    for(int inj = interval_start; inj <= interval_end && *running; inj += interval_step)
    {


        vdac.SetParameter("inject", uint(inj));
        config.SendUpdate(Configuration::vdac);
        config.RestartInjections(true);
        ++stepcounter;
        std::cout << ".";
        std::cout.flush();

        Timing::Sleep(sleep); //225 + 25 * numsignals / 100); //TODO: too long?

        if(stepcounter >= maxsteps)
        {
            stepcounter = 0;
            std::vector<Dataset> newhits = config.ReadoutAll(running, false, uint(maxsteps * numsignals * 2), 10, readsize);
            if(newhits.size() > 0)
                data.insert(data.end(), newhits.begin(), newhits.end());

            if(newhits.size() == 0)
            {
                inj += interval_step;
                skipped = true;
            }
            else if(skipped)
            {
                inj -= interval_step;
                skipped = false;
            }
        }
    }
    std::cout << std::endl;

    //read out the remaining hits:
    Timing::Sleep(7*sleep);
    std::vector<Dataset> newhits = config.ReadoutAll(running, false, uint(maxsteps * numsignals * 2), 20, readsize);
    if(newhits.size() > 0)
        data.insert(data.end(), newhits.begin(), newhits.end());

    //container for collecting the read hits:
    std::stringstream sdata("");
    sdata << Dataset::GetStringHeader(true) << std::endl;

    for(auto& it : data)
    {
        sdata << it.ToString(true) << std::endl; //collect the hits for debugging

        auto sc = scurves.find(it);
        if(sc != scurves.end())
        {
            sc->second.AddHits(1.8 * (interval_start + (it.triggerid - 1) / numsignals * interval_step) / 255., 1);
                //trigger ID starts at 1
        }
        else
        {
            int step = (it.triggerid - 1) / numsignals; //trigger ID starts at 1
            SCurve curve;
            for(int i = 0; i < step; ++i)
                curve.AddPoint(1.8 * (interval_start + i * interval_step) / 255., 0);
            curve.AddPoint(1.8 * (interval_start + step * interval_step) / 255., 1);

            scurves.insert(std::make_pair(it, curve));
        }
    }

    //Write the read hits to a file [TESTING feature, TODO: delete this]:
    std::string filename = Configuration::FindFileName("SCurve_Fast_data_",".dat");
    std::fstream f;
    f.open(filename.c_str(), std::ios::out | std::ios::app);
    f << sdata.str();
    f.flush();
    f.close();
    std::cout << "Data size: " << data.size() << " -> \"" << filename << "\"" << std::endl;

    //restore the back-uped configurations:
    config.SetInjectionConfig(injconf_backup);
    injconf_backup = nullptr;
    config.SetATLASPixConfig(Configuration::vdac, vdac_backup);
    vdac_backup = nullptr;

    //transmit the settings to FPGA:
    config.ConfigureInjections(false);
    config.SendUpdate(Configuration::vdac);
    config.GetNexysIO()->Flush();

    return scurves;
}

std::map<Dataset, SCurve> Trimming::MeasureSCurves(bool* running, bool updategui, int activepixels)
{
    if(running == nullptr || *running == false)
        return std::map<Dataset, SCurve>();

    bool selfblocked = !config.GetProgressBarBlocked();
    if(selfblocked)
        config.BlockProgressBar();

    //backup injection settings:
    Injection_Config* injconf_backup = config.GetInjectionConfig();
    if(injconf_backup == nullptr)
    {
        std::cerr << "Error: Injection config empty, aborting SCurve." << std::endl;
        return std::map<Dataset,SCurve>();
    }
    Injection_Config injconf = *injconf_backup;
    config.SetInjectionConfig(&injconf);
    injconf.SetNumPulseSets(static_cast<unsigned int>(numsignals));

    //backup VDAC settings:
    ASIC_Config2* vdac_backup = config.GetATLASPixConfig(Configuration::vdac);
    if(vdac_backup == nullptr)
    {
        std::cerr << "Error: VDAC config empty, aborting SCurve" << std::endl;
        return std::map<Dataset, SCurve>();
    }
    ASIC_Config2 vdac = *vdac_backup;
    config.SetATLASPixConfig(Configuration::vdac, &vdac);

    //SCurve container:
    std::map<Dataset, SCurve> scurves;

    //prepare the injections
    int splitinjections = 1;
    int numsignals_backup = numsignals;
    const int maxnuminjections = 120;
    if(injconf.GetNumPulseSets() > maxnuminjections)
    {
        splitinjections = injconf.GetNumPulseSets() / maxnuminjections + 1;
        injconf.SetNumPulseSets(maxnuminjections);
        //numsignals = maxnuminjections * splitinjections;
    }
    config.StopInjections(false);
    config.ConfigureInjections(true);

    if(selfblocked)
    {
        config.SetProgressBarMaximum((interval_end - interval_start) / interval_step + 1);
        config.SetProgressBarValue(0);
    }

    //calculate injection duration with security factor 1.5:
    //int sleepduration = int(numsignals * (int(injconf.GetClockDiv()) * int(injconf.GetNumPulsesInaSet())
    //                                  * int(injconf.GetPeriod()) + int(injconf.GetInitDelay()))
    //                        / 100. * 1.5) + 1; //duration in ms @ 100MHz

    SCurve scdummy;
    scdummy.SetMaximum(numsignals);
    int skippoints = 0;

    //not used
    //reset the trigger ID:
    //config.GetNexysIO()->Write(43,1,false,4);
    //config.GetNexysIO()->Write(43,0,true,1);

    int pbstartvalue     = config.GetProgressBarValue();
    int stepsdone        = 0;   //number of measurement steps already performed (or skipped permanently)
    bool lasttimeskipped = false;

    int lastvalue = interval_start;
    bool redostep = false;

    for(int i = interval_start; i <= interval_end && *running; i += interval_step)
    {
        if(redostep)
        {
            redostep = false;
            i = lastvalue;
        }
        else
            lastvalue = i;

        //set injection voltage:
        //injconf.SetInitDelay(uint(inj));//ivan
        vdac.SetParameter("inject", static_cast<unsigned int>(i));
        config.SendUpdate(Configuration::vdac);
        std::cout << "." << std::flush;


        config.StopFastReadout(false);//Ivan
        //config.GetNexysIO()->Write(0, NexysIO::Ld, false, 500);
        config.ResetFastReadout(-1, false);
        //config.GetNexysIO()->Write(0, NexysIO::Ld, false, 500); //just as delay between reset and injection start
        config.StartFastReadout(true, false);//Ivan
        //config.GetNexysIO()->Write(0, NexysIO::Ld, false, 500);//Ivan
        Timing::Sleep(50);


        std::vector<Dataset> hitlist;

        //set the number of injections to the maximum if more are requested:
        if(splitinjections > 1)
        {
            injconf.SetNumPulseSets(maxnuminjections);
            config.ConfigureInjections(false);
        }

        for(int i = 0; i < splitinjections; ++i)
        {
            //do the remaining injections on last subset:
            if(i + 1 == splitinjections && splitinjections > 1)
            {
                injconf.SetNumPulseSets(uint(numsignals_backup - maxnuminjections * i));

                if(injconf.GetNumPulseSets() == 0)
                    break;

                config.ConfigureInjections(false);
            }

            config.RestartInjections(true);

            Timing::Sleep(10);

            //read the data from the FPGA:
            std::vector<Dataset> hits = config.ReadoutAll(nullptr, false,
                                                             //AP3columns * static_cast<unsigned int>(numsignals) * 20,
                                                          static_cast<unsigned int>(numsignals) * 5,
                                                          ((i < splitinjections - 1)?2:5), 5, //?2:10), 5);
                                                          maxtimeperpoint);


          //  std::vector<Dataset> Configuration::ReadoutAll(bool *running, bool print, unsigned int maxnumhits,
                                                      //     int maxemptyreads, int readsize, int maxtime)



            hitlist.insert(hitlist.end(), hits.begin(), hits.end());
        }

        std::map<Dataset, int> sortedhits = config.SortHits(hitlist, false, usetrigID);


        //int maxcol = -1;
        //int maxrow = -1;
        //int maxhits = -1;
        std::map<int, int> changedrows;
        for(auto& it : sortedhits)
        {
            if(it.second > 3 * numsignals) // && it.second > maxhits) //to exclude only the worst pixel
            {
                //maxcol = it.first.column;
                //maxrow = it.first.row;
                //maxhits = it.second;
                config.GetTDACConfig()->SetTDACValue(uint(it.first.column), uint(it.first.row),
                                static_cast<unsigned int>(config.GetTDACConfig()->GetTDACValue(uint(it.first.column), uint(it.first.row)) + 1));
                if(changedrows.find(it.first.row) == changedrows.end())
                    changedrows.insert(std::make_pair(it.first.row, 1));
                else
                    ++(changedrows[it.first.row]);
                std::cout << "Detected noise on pixel " << it.first.ToAddressString()
                          << " with " << it.second << " signals." << std::endl;
            }
        }
        //if(maxcol >= 0)
        // for more than 6 rows, the threshold change delay takes longer than writing the matrix as a whole:
        //   6 * 500ms = 3s -> about total time for matrix writing
        if(changedrows.size() > 6)
        {
            config.WriteRAMMatrix();
            std::cout << "redo step at " << i << " due to noise on " << changedrows.size() << " rows" << std::endl;
            redostep = true;
            continue;
        }
        else if(changedrows.size() > 0)
        {
            for(auto it = changedrows.begin(); it != changedrows.end(); ++it)
                config.WriteRAMRow(it->first, true);

            std::cout << "redo step at " << i << " due to noise on " << changedrows.size() << " rows" << std::endl;
            redostep = true;
            //deactivated_pixels.push_back(std::make_pair(col, row));
            continue;
        }


        //transfer the data to the SCurve containers:
        int efficientpixelcounter = 0;
        for(auto& it : sortedhits)//sorted hits hits
        {
            auto sc = scurves.find(it.first);//scurves empty
            if(sc != scurves.end())
                sc->second.AddPoint(i/255.*1.8, it.second);
            else
            {
                SCurve curve = scdummy;//scurve with all 0
                curve.AddPoint(i/255.*1.8, it.second);//starts from higher inj where there are hits
                scurves.insert(std::make_pair(it.first, curve));//making scurves
            }

            if(it.second >= numsignals)
                ++efficientpixelcounter;
        }//at the end the scurves is filled with values from the hit occurance

        //add empty points to SCurves without new hits:
        for(auto& it : scurves)
        {
            if(it.second.GetNumPoints() <= scdummy.GetNumPoints())
                it.second.AddPoint(i/255.*1.8, 0);
        }


        //fill the dummy scurve with the point measured now as a start for the pixels
        //  becoming efficient with larger signals:
        scdummy.AddPoint(i/255.*1.8, 0);

        //skip points if nothing was measured:
        //  on no measured points skip the two next points,
        //  if something was measured during skipping also perform the measurements
        //  on the last skipped points.
        //  After the expected number of pixels is efficient (`activepixels`)
        //  the measurement distance is tripled again. The feature is off for
        //    activepixels <= 0 (default)
        //
        //  Measurement order (x - skipped):
        //   ... 1 x x 2 x x 3 5 6 4 7 x x 8 x x 9 ...
        //                         ,---------------     ^ efficiency
        //                        /                     |
        //       _______________.´                      |
        //
        // the number of measurement points skipped can be adjusted by changing `numskip`
        //   which contains the gap size

        const int numskip = 2;
        if(skippoints >= 2 && skippoints <= numskip)  // <= (numskip - 2 + //
            ++skippoints;//3
        //skip the already made measurement (index 4 above):
        else if(skippoints > numskip && numskip > 0)
        {
            scdummy.Sort();

            skippoints = 0;
            i += interval_step;
        }
        //skip `numskip` data points:
        else if(sortedhits.size() == 0 || (activepixels > 0 && efficientpixelcounter >= (activepixels-1)))//before transition and after transition ivan
        {
            skippoints = 1;
            i += interval_step * numskip;

            //add permanently skipped points to the step counter:
            if(lasttimeskipped)
                stepsdone += numskip;
            lasttimeskipped = true; //to mark that there were skipped points the last time
        }
        //go back `numskip` data points to close the gap:
        else if(skippoints == 1 && numskip > 0) // && sortedhits.size() > 0)
        {
            skippoints = 2;
            i -= (numskip + 1) * interval_step;//go back by 2 to first skipped !
            lasttimeskipped = false; //to mark that the skipped points will be measured
        }
        else if(numskip <= 0)
            skippoints = 0;

        ++stepsdone;
        if(updategui)
        {
            config.SetProgressBarValue(pbstartvalue + stepsdone);
            config.ProcessEvents();
        }
    }
    std::cout << std::endl;
    //make sure the progressbar matches the number of steps perfomed + skipped:
    config.SetProgressBarValue(pbstartvalue + (interval_end - interval_start) / interval_step + 1);

    //repair the measurement order from skipped points:
    for(auto& it : scurves)
        it.second.Sort();

    //restore the back-uped configurations:
    config.SetInjectionConfig(injconf_backup);
    injconf_backup = nullptr;
    config.SetATLASPixConfig(Configuration::vdac, vdac_backup);
    vdac_backup = nullptr;

    if(selfblocked)
        config.ReleaseProgressBar();

    numsignals = numsignals_backup;

    return scurves;
}


std::map<Dataset, SCurve> Trimming::MeasureSCurvesDelay(bool* running, bool updategui, int activepixels)
{
    if(running == nullptr || *running == false)
        return std::map<Dataset, SCurve>();

    bool selfblocked = !config.GetProgressBarBlocked();
    if(selfblocked)
        config.BlockProgressBar();

    //backup injection settings:
    Injection_Config* injconf_backup = config.GetInjectionConfig();
    if(injconf_backup == nullptr)
    {
        std::cerr << "Error: Injection config empty, aborting SCurve." << std::endl;
        return std::map<Dataset,SCurve>();
    }
    Injection_Config injconf = *injconf_backup;
    config.SetInjectionConfig(&injconf);
    injconf.SetNumPulseSets(static_cast<unsigned int>(numsignals));

    //backup VDAC settings:
    ASIC_Config2* vdac_backup = config.GetATLASPixConfig(Configuration::vdac);
    if(vdac_backup == nullptr)
    {
        std::cerr << "Error: VDAC config empty, aborting SCurve" << std::endl;
        return std::map<Dataset, SCurve>();
    }
    ASIC_Config2 vdac = *vdac_backup;
    config.SetATLASPixConfig(Configuration::vdac, &vdac);

    //SCurve container:
    std::map<Dataset, SCurve> scurves;

    //prepare the injections
    int splitinjections = 1;
    int numsignals_backup = numsignals;
    const int maxnuminjections = 120;
    if(injconf.GetNumPulseSets() > maxnuminjections)
    {
        splitinjections = injconf.GetNumPulseSets() / maxnuminjections + 1;
        injconf.SetNumPulseSets(maxnuminjections);
        //numsignals = maxnuminjections * splitinjections;
    }
    config.StopInjections(false);
    config.ConfigureInjections(true);

    if(selfblocked)
    {
        config.SetProgressBarMaximum((interval_end - interval_start) / interval_step + 1);
        config.SetProgressBarValue(0);
    }

    //calculate injection duration with security factor 1.5:
    //int sleepduration = int(numsignals * (int(injconf.GetClockDiv()) * int(injconf.GetNumPulsesInaSet())
    //                                  * int(injconf.GetPeriod()) + int(injconf.GetInitDelay()))
    //                        / 100. * 1.5) + 1; //duration in ms @ 100MHz

    SCurve scdummy;
    scdummy.SetMaximum(numsignals);
    int skippoints = 0;

    //not used
    //reset the trigger ID:
    //config.GetNexysIO()->Write(43,1,false,4);
    //config.GetNexysIO()->Write(43,0,true,1);

    int pbstartvalue     = config.GetProgressBarValue();
    int stepsdone        = 0;   //number of measurement steps already performed (or skipped permanently)
    bool lasttimeskipped = false;

    int lastvalue = interval_start;
    bool redostep = false;

    //for(int i = 0; i <= 31; i += 1)
    for(int i = interval_start; i <= interval_end && *running; i += interval_step)

    {
        if(redostep)
        {
            redostep = false;
            i = lastvalue;
        }
        else
            lastvalue = i;

        //set injection voltage:
        injconf.SetInitDelay(uint(63-i));//ivan
        config.ConfigureInjections(false);
        //vdac.SetParameter("inject", static_cast<unsigned int>(i));
        //config.SendUpdate(Configuration::vdac);
        std::cout << "." << std::flush;

        config.StopFastReadout(false);//Ivan
        //config.GetNexysIO()->Write(0, NexysIO::Ld, false, 500);
        config.ResetFastReadout(-1, false);
        //config.GetNexysIO()->Write(0, NexysIO::Ld, false, 500); //just as delay between reset and injection start
        config.StartFastReadout(true, false);//Ivan
        //config.GetNexysIO()->Write(0, NexysIO::Ld, false, 500);//Ivan
        Timing::Sleep(50);

        std::vector<Dataset> hitlist;

        //set the number of injections to the maximum if more are requested:
        if(splitinjections > 1)
        {
            injconf.SetNumPulseSets(maxnuminjections);
            config.ConfigureInjections(false);
        }

        for(int i = 0; i < splitinjections; ++i)
        {
            //do the remaining injections on last subset:
            if(i + 1 == splitinjections && splitinjections > 1)
            {
                injconf.SetNumPulseSets(uint(numsignals_backup - maxnuminjections * i));

                if(injconf.GetNumPulseSets() == 0)
                    break;

                config.ConfigureInjections(false);
            }

            config.RestartInjections(true);

            Timing::Sleep(10);

            //read the data from the FPGA:
            std::vector<Dataset> hits = config.ReadoutAll(nullptr, false,
                                                             //AP3columns * static_cast<unsigned int>(numsignals) * 20,
                                                          static_cast<unsigned int>(numsignals) * 5,
                                                          ((i < splitinjections - 1)?2:5), 5, //?2:10), 5);
                                                          maxtimeperpoint);

            hitlist.insert(hitlist.end(), hits.begin(), hits.end());
        }

        std::map<Dataset, int> sortedhits = config.SortHits(hitlist, false, usetrigID);


        //int maxcol = -1;
        //int maxrow = -1;
        //int maxhits = -1;
        std::map<int, int> changedrows;
        for(auto& it : sortedhits)
        {
            if(it.second > 3 * numsignals) // && it.second > maxhits) //to exclude only the worst pixel
            {
                //maxcol = it.first.column;
                //maxrow = it.first.row;
                //maxhits = it.second;
                config.GetTDACConfig()->SetTDACValue(uint(it.first.column), uint(it.first.row),
                                static_cast<unsigned int>(config.GetTDACConfig()->GetTDACValue(uint(it.first.column), uint(it.first.row)) + 1));
                if(changedrows.find(it.first.row) == changedrows.end())
                    changedrows.insert(std::make_pair(it.first.row, 1));
                else
                    ++(changedrows[it.first.row]);
                std::cout << "Detected noise on pixel " << it.first.ToAddressString()
                          << " with " << it.second << " signals." << std::endl;
            }
        }
        //if(maxcol >= 0)
        // for more than 6 rows, the threshold change delay takes longer than writing the matrix as a whole:
        //   6 * 500ms = 3s -> about total time for matrix writing
        if(changedrows.size() > 6)
        {
            config.WriteRAMMatrix();
            std::cout << "redo step at " << i << " due to noise on " << changedrows.size() << " rows" << std::endl;
            redostep = true;
            continue;
        }
        else if(changedrows.size() > 0)
        {
            for(auto it = changedrows.begin(); it != changedrows.end(); ++it)
                config.WriteRAMRow(it->first, true);

            std::cout << "redo step at " << i << " due to noise on " << changedrows.size() << " rows" << std::endl;
            redostep = true;
            //deactivated_pixels.push_back(std::make_pair(col, row));
            continue;
        }


        //transfer the data to the SCurve containers:
        int efficientpixelcounter = 0;
        for(auto& it : sortedhits)
        {
            auto sc = scurves.find(it.first);
            if(sc != scurves.end())
                sc->second.AddPoint(i, it.second);
            else
            {
                SCurve curve = scdummy;
                curve.AddPoint(i, it.second);
                scurves.insert(std::make_pair(it.first, curve));
            }

            if(it.second >= numsignals)
                ++efficientpixelcounter;
        }

        //add empty points to SCurves without new hits:
        for(auto& it : scurves)
        {
            if(it.second.GetNumPoints() <= scdummy.GetNumPoints())
                it.second.AddPoint(i, 0);
        }


        //fill the dummy scurve with the point measured now as a start for the pixels
        //  becoming efficient with larger signals:
        scdummy.AddPoint(i, 0);

        //skip points if nothing was measured:
        //  on no measured points skip the two next points,
        //  if something was measured during skipping also perform the measurements
        //  on the last skipped points.
        //  After the expected number of pixels is efficient (`activepixels`)
        //  the measurement distance is tripled again. The feature is off for
        //    activepixels <= 0 (default)
        //
        //  Measurement order (x - skipped):
        //   ... 1 x x 2 x x 3 5 6 4 7 x x 8 x x 9 ...
        //                         ,---------------     ^ efficiency
        //                        /                     |
        //       _______________.´                      |
        //
        // the number of measurement points skipped can be adjusted by changing `numskip`
        //   which contains the gap size



        const int numskip = 2;
        if(skippoints >= 2 && skippoints <= numskip)  // <= (numskip - 2 +
            ++skippoints;
        //skip the already made measurement (index 4 above):
        else if(skippoints > numskip && numskip > 0)
        {
            scdummy.Sort();

            skippoints = 0;
            i += interval_step;
        }
        //skip `numskip` data points:
        else if(sortedhits.size() == 0 || (activepixels > 0 && efficientpixelcounter >= (activepixels-1)))//ivan -1
        {
            skippoints = 1;
            i += interval_step * numskip;

            //add permanently skipped points to the step counter:
            if(lasttimeskipped)
                stepsdone += numskip;
            lasttimeskipped = true; //to mark that there were skipped points the last time
        }
        //go back `numskip` data points to close the gap:
        else if(skippoints == 1 && numskip > 0) // && sortedhits.size() > 0)
        {
            skippoints = 2;
            i -= (numskip + 1) * interval_step;
            lasttimeskipped = false; //to mark that the skipped points will be measured
        }
        else if(numskip <= 0)
            skippoints = 0;





        ++stepsdone;
        if(updategui)
        {
            config.SetProgressBarValue(pbstartvalue + stepsdone);
            config.ProcessEvents();
        }
    }
    std::cout << std::endl;
    //make sure the progressbar matches the number of steps perfomed + skipped:
    config.SetProgressBarValue(pbstartvalue + (interval_end - interval_start) / interval_step + 1);

    //repair the measurement order from skipped points:
    for(auto& it : scurves)
        it.second.Sort();

    //restore the back-uped configurations:
    config.SetInjectionConfig(injconf_backup);
    injconf_backup = nullptr;
    config.SetATLASPixConfig(Configuration::vdac, vdac_backup);
    vdac_backup = nullptr;

    if(selfblocked)
        config.ReleaseProgressBar();

    numsignals = numsignals_backup;

    return scurves;
}







std::map<Dataset, SCurve> Trimming::MeasureAllSCurves(bool delayscan, bool *running, bool disableothers, bool updategui, int startcol, int endcol, int startrow, int endrow)
{
    //TODO: implement this matrix scan...
    if(config.GetNexysIO() == nullptr || !config.GetNexysIO()->is_open() || running == nullptr || *running == false)
        return std::map<Dataset,SCurve>();

    std::map<Dataset, SCurve> scurves;

    config.logit("Started SCurve Measurement for all pixels...");

    bool selfblocked = !config.GetProgressBarBlocked();
    if(selfblocked)
        config.BlockProgressBar();

    TDAC_Config* tdac_backup = config.GetTDACConfig();
    TDAC_Config tdac_scurve = *tdac_backup;
    config.SetTDACConfig(&tdac_scurve);

    if(disableothers)
    {
        for(unsigned int disablerow = 0; disablerow < AP3rows; ++disablerow)
            for(unsigned int disablecol = 0; disablecol < AP3columns; ++disablecol)
                config.GetTDACConfig()->SetTDACValue(disablecol, disablerow, 15);

        if(!config.WriteRAMMatrix())
            return std::map<Dataset, SCurve>();
    }

    if(selfblocked)
    {
        config.SetProgressBarMaximum(endrow - startrow + 1);
        config.SetProgressBarValue(0);
        config.ProcessEvents();
    }

    for(unsigned int row = uint(startrow); row <= uint(endrow) && *running; ++row)
    {
        if(disableothers)
        {
            for(unsigned int disablecol = 0; disablecol < AP3columns; ++disablecol)
            {
                config.GetTDACConfig()->SetTDACValue(disablecol, row - 1, 15);
                config.GetTDACConfig()->SetTDACValue(disablecol, row,      0);
            }
            if(row > 0)
                config.WriteRAMRow(int(row) - 1);
            config.WriteRAMRow(int(row));
            RemoveNoise();
            //if(RemoveNoise() < 0)
            //    return std::map<Dataset, SCurve>();
        }

        std::stringstream s1("");
        s1 << "start SCurve row " << row;
        config.logit(s1.str());

        for(int stcol = startcol; stcol <= endcol; stcol += maxsimultaneouspixels)//ivan changed from <
        {
            int edcol = stcol + maxsimultaneouspixels - 1;
            if(edcol > endcol)
                edcol = endcol;
            ConfigureInjections(int(row), true, stcol, edcol); //startcol, endcol);

            std::cout << "Cols " << stcol << " - " << edcol << " ";

            std::map<Dataset, SCurve> newscurves;
            //ivan
            if(delayscan) newscurves = MeasureSCurvesDelay(running, false, edcol - stcol + 1); //endcol - startcol + 1);
            else newscurves = MeasureSCurves(running, false, edcol - stcol + 1); //endcol - startcol + 1);



            //remove noise hits:
            auto it = newscurves.begin();
            int deletecounter = 0;
            while(it != newscurves.end())
            {
                if(it->first.row != int(row) || it->first.column < stcol || it->first.column > edcol)
                {
                    ++deletecounter;
                    if(deletecounter < 20)
                        std::cout << "erasing SCurve for pixel (" << it->first.column << "|"
                                  << it->first.row << ") with " << it->second.GetNumPoints()
                                  << " points" << std::endl;
                    else if(deletecounter == 20)
                        std::cout << "more than " << deletecounter << " SCurves erased..."
                                  << std::endl;
                    it = newscurves.erase(it);
                }
                else
                    ++it;
            }

            if(newscurves.size() > 0)
            {
                for(auto it = newscurves.begin(); it != newscurves.end(); ++it)
                {
                    Dataset dat = it->first;
                    dat.tdac = short(config.GetTDACConfig()->GetTDACValue(uint(dat.column), uint(dat.row)));
                    scurves.insert(std::make_pair(dat, it->second));
                }

                //scurves.insert(newscurves.begin(), newscurves.end());
            }
        }

        std::stringstream s("");
        s << "   finished row " << row;
        config.logit(s.str());
        if(updategui)
        {
            config.SetProgressBarValue(config.GetProgressBarValue() + 1);
            config.ProcessEvents();
        }
    }

    config.logit("Finished SCurve Scan of whole matrix");
    config.SetTDACConfig(tdac_backup);
    tdac_backup = nullptr;

    if(selfblocked)
        config.ReleaseProgressBar();

    return scurves;
}

std::map<Dataset, SCurve> Trimming::MeasureSparseSCurves(bool *running, int numrows, bool updategui, Rect area, int randinit, bool delay)
{
    return MeasureSparseSCurves(running, numrows, updategui, area.startcol, area.endcol,
                                area.startrow, area.endrow, randinit, delay);
}

std::map<Dataset, SCurve> Trimming::MeasureSparseSCurves(bool *running, int numrows, bool updategui,
                                                         int startcol, int endcol, int startrow, int endrow, int randinit, bool delay)
{
    if(running == nullptr || *running == false)
        return std::map<Dataset, SCurve>();

    //for too many testing rows measure the complete matrix range specified:
    if(numrows >= endrow - startrow + 1)
        return MeasureAllSCurves(delay, running, false, updategui, startcol, endcol, startrow, endrow);


    //store the measured rows to measure a row only once:
    static bool initialised = false;
    if(randinit != 0 || !initialised)
    {
        initialised = true;
        if(randinit == 0)
            srand(uint(time(nullptr)));
        else
            srand(uint(randinit));
    }

    std::vector<int> measurerows;

    for(int i = 0; i < numrows; ++i)
    {
        int row = 0;
        int failcounter = 0;
        do
        {
            row = (rand() % (endrow - startrow + 1)) + startrow;
            for(auto& it : measurerows)
            {
                if(it == row)
                {
                    row = -1;
                    break;
                }
            }

            //abort search on too often already used row indices:
            if(failcounter > 300)
                return std::map<Dataset, SCurve>();

        }while(row < 0);

        measurerows.push_back(row);
    }

    return MeasureSparseSCurves(running, measurerows, updategui, startcol, endcol, delay);
}

std::map<Dataset, SCurve> Trimming::MeasureSparseSCurves(bool *running, std::vector<int> rows, bool updategui, int startcol, int endcol, bool delay)
{
    if(running == nullptr || *running == false)
        return std::map<Dataset, SCurve>();

    std::map<Dataset, SCurve> scurves;

    if(updategui)
        config.logit("Started Sparse SCurve Measurement ...");
    else
        std::cout << "  Started Sparse SCurve Measurement ..." << std::endl;

    bool selfblocked = !config.GetProgressBarBlocked() && updategui;

    if(selfblocked)
    {
        config.BlockProgressBar();
        config.SetProgressBarMaximum(int(rows.size()) * ((interval_end - interval_start) / interval_step + 1));
        config.SetProgressBarValue(0);
        config.ProcessEvents();
    }

    int index = 0;
    for(auto& row : rows)
    {
        if(*running == false)
            break;

        std::cout << "     measure row " << row << " (" << ++index << "/" << rows.size() << ") ..." << std::endl;

        for(int stcol = startcol; stcol < endcol; stcol += maxsimultaneouspixels)
        {
            int edcol = stcol + maxsimultaneouspixels - 1;
            if(edcol > endcol)
                edcol = endcol;
            ConfigureInjections(row, true, stcol, edcol); //startcol, endcol);

            std::cout << "Cols " << stcol << " - " << edcol << " ";
            std::map<Dataset, SCurve> newscurves;
            if(delay) newscurves = MeasureSCurvesDelay(running, updategui, edcol - stcol + 1); //endcol - startcol + 1);
            else newscurves = MeasureSCurves(running, updategui, edcol - stcol + 1); //endcol - startcol + 1);

            //remove noise hits:
            auto it = newscurves.begin();
            while(it != newscurves.end())
            {
                if(it->first.row != int(row) || it->first.column < stcol || it->first.column > edcol)
                {
                    if(updategui)
                        std::cout << "erasing SCurve for pixel (" << it->first.column << "|"
                                  << it->first.row << ") with " << it->second.GetNumPoints()
                                  << " points" << std::endl;
                    it = newscurves.erase(it);
                }
                else
                    ++it;
            }

            //decorate new scurves with TDAC value used and add them to the list:
            if(newscurves.size() > 0)
            {
                for(auto it = newscurves.begin(); it != newscurves.end(); ++it)
                {
                    Dataset dat = it->first;
                    dat.tdac = short(config.GetTDACConfig()->GetTDACValue(uint(dat.column), uint(dat.row)));
                    scurves.insert(std::make_pair(dat, it->second));
                }
            }
        }
    }

    if(selfblocked)
        config.ReleaseProgressBar();

    return scurves;
}

int Trimming::RemoveNoise()
{
    if(config.GetNexysIO() == nullptr || !config.GetNexysIO()->is_open())
        return -1;

    bool selfblocked = !config.GetProgressBarBlocked();
    if(selfblocked)
        config.BlockProgressBar();

    int increaseconter = 0;
    const int maxincreases = 2000;


    if(selfblocked)
    {
        config.SetProgressBarMaximum(maxincreases);
        config.SetProgressBarValue(0);
        config.ProcessEvents();
    }

    while(increaseconter < maxincreases)
    {
        config.ResetFastReadout(FastReadout::fifoclear, true);

        Timing::Sleep(1000);
        std::vector<Dataset> data = config.ReadoutAll(nullptr,false, 2000);

        if(data.size() == 0)
            break;

        std::map<Dataset, int> sorteddata = config.SortHits(data);

        for(auto& it : sorteddata)
        {
            unsigned int col = static_cast<unsigned int>(it.first.column);
            unsigned int row = static_cast<unsigned int>(it.first.row);

            if(config.GetTDACConfig()->GetTDACValue(col, row) < 8)
            {
                config.GetTDACConfig()->SetTDACValue(col, row, static_cast<unsigned int>(config.GetTDACConfig()->GetTDACValue(col, row) + 1));
                config.WriteRAMRow(int(row), true);
                ++increaseconter;
            }
            //else
            //    return -2;
        }

        if(selfblocked)
        {
            config.SetProgressBarValue(increaseconter);
            config.ProcessEvents();
        }
    }

    config.ResetFastReadout(FastReadout::fifoclear, true);

    if(selfblocked)
    {
        config.SetProgressBarValue(maxincreases);
        config.ProcessEvents();
        config.ReleaseProgressBar();
    }

    return increaseconter;
}

void scurve_fvec(const alglib::real_1d_array &x, alglib::real_1d_array &fi, void *ptr)
{
    std::map<double, double>* fitcurve = static_cast<std::map<double, double>* >(ptr);

    auto it = fitcurve->begin();
    for(unsigned int i = 0; i < fitcurve->size(); ++i)
    {
        fi[int(i)] = SCurve::EvalSCurve(it->first, x[0], x[1]) - it->second;
        ++it;
    }
}
void scurve_jac(const alglib::real_1d_array &x, alglib::real_1d_array &fi, alglib::real_2d_array &jac, void *ptr)
{
    std::map<double, double>* fitcurve = static_cast<std::map<double, double>* >(ptr);

    auto it = fitcurve->begin();
    for(unsigned int i = 0; i < fitcurve->size(); ++i)
    {
        fi[int(i)] = SCurve::EvalSCurve(it->first, x[0], x[1]) - it->second;

        jac[int(i)][0] = -exp(pow((it->first-x[0])/x[1],2)) / sqrt(3.14159265);
        jac[int(i)][1] = jac[int(i)][0] * (it->first - x[0]) / x[1];
        jac[int(i)][0] /= x[1];
        ++it;
    }
}

void window_fvec(const alglib::real_1d_array &x, alglib::real_1d_array &fi, void *ptr)
{
    std::map<double, double>* fitcurve = static_cast<std::map<double, double>* >(ptr);

    auto it = fitcurve->begin();
    for(unsigned int i = 0; i < fitcurve->size(); ++i)
    {
        fi[int(i)] = SCurve::EvalWindow(it->first, x[0], x[1], x[2]) - it->second;
        ++it;
    }
}
void window_jac(const alglib::real_1d_array &x, alglib::real_1d_array &fi, alglib::real_2d_array &jac, void *ptr)
{
    std::map<double, double>* fitcurve = static_cast<std::map<double, double>* >(ptr);

    auto it = fitcurve->begin();
    for(unsigned int i = 0; i < fitcurve->size(); ++i)
    {
        fi[int(i)] = SCurve::EvalWindow(it->first, x[0], x[1], x[2]) - it->second;

        jac[int(i)][0] = 0.5/sqrt(3.14159265)/x[2]*exp(-pow((it->first-x[0])/x[2],2))*(1+erf((x[1]-it->first)/x[2]));
        jac[int(i)][1] = -0.5/sqrt(3.14159265)/x[2]*exp(-pow((x[1]-it->first)/x[2],2))*(1+erf((it->first-x[0])/x[2]));
        jac[int(i)][2] = 0.5 / sqrt(3.14159265) / pow(x[2], 2)
                            * ( (x[1] - it->first) * exp(-pow((x[1] - it->first) / x[2], 2)) * (1 + erf((it->first - x[1]) / x[2]))
                              + (it->first - x[0]) * exp(-pow((it->first - x[0]) / x[2], 2)) * (1 + erf((x[1] - it->first) / x[2])));

        ++it;
    }
}

bool Trimming::FitSingleSCurve(SCurve &data, bool print, double threshold)
{
    if(data.GetNumPoints() < 3)
    {
        data.SetNoise(1e10);
        data.SetThreshold(1e10);
        return false;
    }

    std::map<double, double>* curve = new std::map<double, double>(data.GetNormedSortedMap());

    //set starting values:
    alglib::real_1d_array x;
    x.setlength(2);
    x[0] = 0;
    x[1] = 0.5;

    //evaluate better starting values to spped up the process:

    //rudolf
    /*
    int pntcnt = 0;
    double first200 = -1e10;
    for(auto& it : *curve)
    {
        if(it.second > 0 && it.second < 0.5)
        {
            x[0] += it.first;
            ++pntcnt;
        }
        if(it.second == 0.5 && std::abs(first200 - (-1e10)) < 1e-3)
            first200 = it.first;
    }
    if(pntcnt > 0)
    {
        x[0] = x[0]/pntcnt + 0.5 * interval_step;
        x[1] = interval_step * (pntcnt + 1);
    }
    else if(std::abs(first200 - (-1e10)) < 1e-3)
    {
        x[0] = first200 - (interval_step / 2);
        x[1] = interval_step;
    }
    else
        x[0] = threshold;
*/


    x[0] = 0;

    double maxx = -1e10;

    for(auto& it : *curve)
    {

       if(it.first > maxx){
        maxx = it.first;
        if(it.second < 0.5) x[0] = it.first;
       }

    }

    x[1] = interval_step;



    //set termination conditions:
    //double epsg = 0;
    //double epsf = 1e-10;
    //double epsx = 0;
    alglib::ae_int_t maxits = 1000;

    //containers for state and result:
    alglib::minlmstate  state;
    alglib::minlmreport report;

    //fitting:
    alglib::minlmcreatev(int(curve->size()), x, 1.e-4, state);
    alglib::minlmsetcond(state, 1e-10, maxits);
    //alglib::minlmsetcond(state, epsg, epsf, epsx, maxits);
    alglib::minlmoptimize(state, scurve_fvec, scurve_jac, nullptr, static_cast<void*>(curve));
    alglib::minlmresults(state, x, report);

    //write out information about the fit:
    if(print)
    {
        std::cout << "Fit Results:\n"
                  << "    Iterations:   " << report.iterationscount << std::endl
                  << "    Termination:  " << int(report.terminationtype) << std::endl
                  << "    Parameters:   x0 = " << x[0] << "; width = " << x[1] << std::endl;
    }

    delete curve;

    if(report.iterationscount > 0)
    {
        data.SetThreshold(x[0]);
        data.SetNoise(x[1]);
        return true;
    }
    else
    {
        data.SetThreshold(1e10);
        data.SetNoise(1e10);
        return false;
    }
}

double Trimming::FitSCurves(std::map<Dataset, SCurve> &scurves, bool print, double threshold, bool updategui)
{
    int counter = 0;
    int index = 0;

    bool selfblocked = !config.GetProgressBarBlocked();
    if(selfblocked)
    {
        config.BlockProgressBar();
        config.SetProgressBarMaximum(int(scurves.size()));
        config.SetProgressBarValue(0);
        config.ProcessEvents();
    }

    for(auto& it : scurves)
    {
        if(print)
            std::cout << " ... fitting curve " << ++index << "/" << scurves.size() << std::endl;
        if(FitSingleSCurve(it.second, false, threshold))
                ++counter;

        if(updategui)
        {
            config.SetProgressBarValue(config.GetProgressBarValue() + 1);
            config.ProcessEvents();
        }
    }

    if(print)
        std::cout << " Fitting statistics: " << counter << "/" << scurves.size()
                  << " fits successful" << std::endl;

    if(selfblocked)
    {
        config.SetProgressBarValue(int(scurves.size()));
        config.ProcessEvents();
        config.ReleaseProgressBar();
    }

    return counter / double(scurves.size());
}

bool Trimming::FitSingleWindow(SCurve &data, bool print, double threshold, double length)
{
    if(data.GetNumPoints() < 4)
    {
        data.SetWindowNoise(1e10);
        data.SetWindowStart(1e10);
        data.SetWindowEnd(1e10);
        return false;
    }

    std::map<double, double>* curve = new std::map<double, double>(data.GetNormedSortedMap());

    //set starting values:
    alglib::real_1d_array x;
    x.setlength(3);
    x[0] = 0;
    x[1] = 0.5;
    x[2] = 0.6;

    //evaluate better starting values to speed up the process:
    double lastzero  = -1e10;
    double firstmax  = -1e10;
    bool   second    = false; //edge selection indicator: false - rising edge, true - falling edge
    double lastmax   = -1e10;
    double firstzero = -1e10;
    const double delta = 0.1;
    for(auto& it : *curve)
    {
        if(!second)
        {
            //probe rising edge:
            if(it.second < delta)
                lastzero = it.first;
            else if(it.second > 1 - delta)
            {
                firstmax = it.first;
                second = true;;
            }
        }
        else
        {
            //probe falling edge:
            if(it.second > 1 - delta)
                lastmax = it.first;
            else if(it.second < delta)
            {
                firstzero = it.first;
                break;
            }
        }
    }
    //in case something fails, use the provided parameters:
    if(std::abs(lastzero - (-1e10)) < 1e-3 || std::abs(firstmax - (-1e10)) < 1e-3
            || std::abs(lastmax - (-1e10)) < 1e-3 || std::abs(firstzero - (-1e10)) < 1e-3)
    {
        x[0] = threshold;
        x[1] = threshold + length;
        auto it = curve->begin();
        x[2] = it->first;
        ++it;
        x[2] = it->first - x[2];
    }
    else
    {
        x[0] = (lastzero + firstmax) / 2.;
        x[1] = (lastmax + firstzero) / 2.;
        x[2] = (firstmax - lastzero + firstzero - lastmax) / 8.; // / (4*2);
    }


    //set termination conditions:
    //double epsg = 0;
    //double epsf = 1e-10;
    //double epsx = 0;
    alglib::ae_int_t maxits = 1000;

    //containers for state and result:
    alglib::minlmstate  state;
    alglib::minlmreport report;

    //fitting:
    alglib::minlmcreatev(int(curve->size()), x, 1.e-4, state);
    alglib::minlmsetcond(state, 1e-10, maxits);
    //alglib::minlmsetcond(state, epsg, epsf, epsx, maxits);
    alglib::minlmoptimize(state, window_fvec, window_jac, nullptr, static_cast<void*>(curve));
    alglib::minlmresults(state, x, report);

    //write out information about the fit:
    if(print)
    {
        std::cout << "Fit Results:\n"
                  << "    Iterations:   " << report.iterationscount << std::endl
                  << "    Termination:  " << int(report.terminationtype) << std::endl
                  << "    Parameters:   x0 = " << x[0] << "; x1 = " << x[1] << "; width = " << x[2] << std::endl;
    }

    delete curve;

    if(report.iterationscount > 0)
    {
        data.SetWindowStart(x[0]);
        data.SetWindowEnd(x[1]);
        data.SetWindowNoise(x[2]);
        return true;
    }
    else
    {
        data.SetWindowStart(1e10);
        data.SetWindowEnd(1e10);
        data.SetWindowNoise(1e10);
        return false;
    }
}

double Trimming::FitWindows(std::map<Dataset, SCurve> &scurves, bool print, double threshold, double length, bool updategui)
{
    int counter = 0;
    int index = 0;

    bool selfblocked = !config.GetProgressBarBlocked() && updategui;
    if(selfblocked)
    {
        config.BlockProgressBar();
        config.SetProgressBarMaximum(int(scurves.size()));
        config.SetProgressBarValue(0);
        config.ProcessEvents();
    }

    for(auto& it : scurves)
    {
        if(print)
            std::cout << " ... fitting curve " << ++index << "/" << scurves.size() << std::endl;
        if(FitSingleWindow(it.second, false, threshold, length))
                ++counter;

        if(updategui)
        {
            config.SetProgressBarValue(config.GetProgressBarValue() + 1);
            config.ProcessEvents();
        }
    }

    if(print)
        std::cout << " Fitting statistics: " << counter << "/" << scurves.size()
                  << " fits successful" << std::endl;

    if(selfblocked)
    {
        config.SetProgressBarValue(int(scurves.size()));
        config.ProcessEvents();
        config.ReleaseProgressBar();
    }

    return counter / double(scurves.size());
}

Histogram Trimming::FillHistogram(std::map<Dataset, SCurve> &SCurves, double start, double end, double binwidth)
{
    Histogram hist("", horizontal);

    hist.clearHistogram();

    hist.setBinWidth(binwidth);
    hist.setFirstBinMean(start + binwidth/2);
    hist.addBins(uint((end-start)/binwidth));

    for(auto& it : SCurves)
    {
        if(std::abs(it.second.GetThreshold() - (-1)) > 1e-5 && std::abs(it.second.GetThreshold() - 1e10) > 1e-5
                && std::abs(it.second.GetNoise() - (-1)) > 1e-5 && std::abs(it.second.GetNoise() - 1e10) > 1e-5)
            hist.Fill(it.second.GetThreshold());
    }

    hist.setValidity(true);
    return hist;
}


Histogram Trimming::FillHistogramWithNoise(std::map<Dataset, SCurve> &SCurves, double start, double end, double binwidth)
{
    Histogram hist("", horizontal);

    hist.clearHistogram();

    hist.setBinWidth(binwidth);
    hist.setFirstBinMean(start + binwidth/2);
    hist.addBins(uint((end-start)/binwidth));

    for(auto& it : SCurves)
    {
        if(std::abs(it.second.GetThreshold() - (-1)) > 1e-5 && std::abs(it.second.GetThreshold() - 1e10) > 1e-5
                && std::abs(it.second.GetNoise() - (-1)) > 1e-5 && std::abs(it.second.GetNoise() - 1e10) > 1e-5)
            hist.Fill(it.second.GetNoise());
    }

    hist.setValidity(true);
    return hist;
}

double EvalGaus(double x, double mean, double sigma, double amplitude)
{
    return amplitude/sqrt(2*3.14159265)/sigma * exp(-0.5*pow((x-mean)/sigma, 2));
}

void gaus_fvec(const alglib::real_1d_array &x, alglib::real_1d_array &fi, void *ptr)
{
    std::map<double, double>* fitcurve = static_cast<std::map<double, double>* >(ptr);

    auto it = fitcurve->begin();
    for(unsigned int i = 0; i < fitcurve->size(); ++i)
    {
        fi[int(i)] = EvalGaus(it->first, x[0], x[1], x[2]) - it->second;
        ++it;
    }
}
void gaus_jac(const alglib::real_1d_array &x, alglib::real_1d_array &fi, alglib::real_2d_array &jac, void *ptr)
{
    std::map<double, double>* fitcurve = static_cast<std::map<double, double>* >(ptr);

    auto it = fitcurve->begin();
    for(unsigned int i = 0; i < fitcurve->size(); ++i)
    {
        fi[int(i)] = EvalGaus(it->first, x[0], x[1], x[2]) - it->second;

        jac[int(i)][0] = x[2] / sqrt(2 * 3.14159265) / x[1] * exp(-0.5 * pow((x[0] - it->first) / x[1], 2));
        jac[int(i)][1] = x[2] / sqrt(2 * 3.14159265) * (pow((it->first - x[0]) / pow(x[1], 2), 2) - 1. / pow(x[1], 2))
                                * exp(-0.5 * pow((it->first - x[0]) / x[1], 2));
        jac[int(i)][2] = 1. / sqrt(2 * 3.14159265) / x[1] * exp(-pow((x[0] - it->first) / x[1], 2));

        ++it;
    }
}

bool Trimming::FitGaus(Histogram &gaussian, bool print)
{
    if(gaussian.getNumBins() < 4)
    {
        gaussian.SetAmplitude(-1);
        gaussian.SetMean(-1);
        gaussian.SetSigma(-1);
        return false;
    }

    std::map<double, double>* curve = new std::map<double, double>();

    for(unsigned int i = 0; i < gaussian.getNumBins(); ++i)
        curve->insert(std::make_pair(gaussian.getFirstBinMean()+i * gaussian.getBinWidth(), gaussian.getBinContent(i)));


    //set starting values:
    alglib::real_1d_array x;
    x.setlength(3);
    x[0] = 0.5;
    x[1] = 3 * gaussian.getBinWidth();
    double integral = gaussian.getIntegral(false);
    x[2] = integral;

    //evaluate better starting values to spped up the process:
    double maxpos = 0;
    double maximum = -1e10;
    for(auto& it : *curve)
    {
        if(it.second > maximum)
        {
            maxpos = it.first;
            maximum = it.second;
        }
    }

    double hmpos = 0; //half maximum
    double hmvalue = 1e10; //value at half maximum
    for(auto& it : *curve)
    {
        if(std::abs(it.second - maximum/2) < hmvalue)
        {
            hmvalue = std::abs(it.second - maximum/2);
            hmpos = it.first;
        }
    }

    //set the found starting values:
    x[0] = maxpos;
    double sigma = std::abs(hmpos - maxpos) / 1.9218120557;  // /sqrt(2*ln(2));
    x[1] = sigma;

    if(print)
    {
        std::cout << "Gaus-Fit Starting values:"
                  << "\n    mean  = " << x[0]
                  << "\n    sigma = " << x[1]
                  << "\n    ampl  = " << x[2] << std::endl;
    }

    //set termination conditions:
    //double epsg = 0;
    //double epsf = 1e-10;
    //double epsx = 0;
    alglib::ae_int_t maxits = 1000;

    //containers for state and result:
    alglib::minlmstate  state;
    alglib::minlmreport report;

    //fitting:
    alglib::minlmcreatev(int(curve->size()), x, 1.e-4, state);
    alglib::minlmsetcond(state, 1e-10, maxits);
    //alglib::minlmsetcond(state, epsg, epsf, epsx, maxits);
    alglib::minlmoptimize(state, gaus_fvec, gaus_jac, nullptr, static_cast<void*>(curve));
    alglib::minlmresults(state, x, report);

    //write out information about the fit:
    if(print)
    {
        std::cout << "Fit Results:\n"
                  << "    Iterations:   " << report.iterationscount << std::endl
                  << "    Termination:  " << int(report.terminationtype) << std::endl
                  << "    Parameters:   mean = " << x[0] << "; sigma = " << x[1] << "; amplitude = " << x[2] << std::endl;

        std::cout << "Gaus-Fit Result:"
              << "\n    mean  = " << x[0]
              << "\n    sigma = " << x[1]
              << "\n    ampl  = " << x[2] << std::endl;
    }

    delete curve;

    if(report.iterationscount > 0)
    {
        gaussian.SetMean(x[0]);
        gaussian.SetSigma(x[1]);
        gaussian.SetAmplitude(x[2]);
        return true;
    }
    else
    {
        gaussian.SetMean(-1);
        gaussian.SetSigma(-1);
        gaussian.SetAmplitude(-1);
        return false;
    }
}

double Trimming::FindMeanThreshold(bool *running, Rect area, bool updategui,
                                   int sparserows, int random_init, bool delay)
{
    //omit TDAC value 0 and take lower end of distribution at TDAC7:
#ifdef SCANFULLRANGE
    std::cout << "  measure scurves for TDAC=0 ..." << std::endl;
    //measure thresholds for TDAC = 0:
    config.GetTDACConfig()->SetAllToValue(0, 7);
    config.WriteRAMMatrix();
    std::map<Dataset, SCurve> sctdac0;
    sctdac0 = MeasureSparseSCurves(running, sparserows, updategui, area, random_init, delay);
#endif

    std::cout << "  measure scurves for TDAC=7 ..." << std::endl;
    //measure thresholds for TDAC = 7:
    config.GetTDACConfig()->SetAllToValue(7, 7);
    config.WriteRAMMatrix();
    std::map<Dataset, SCurve> sctdac7;
    sctdac7 = MeasureSparseSCurves(running, sparserows, updategui, area, random_init, delay);

#ifdef SCANFULLRANGE
    double efficiency0 = FitSCurves(sctdac0, false, 1.0, false);
#endif
    double efficiency7 = FitSCurves(sctdac7, false, 1.0, false);

    if(updategui)
    {
        std::stringstream se("");
#ifdef SCANFULLRANGE
        se << "  Mean Finding Fitting efficiency " << (efficiency0 + efficiency7) / 2.;
#else
        se << "  Mean Finding Fitting efficiency " << efficiency7;
#endif
        config.logit(se.str());
    }

#ifdef SCANFULLRANGE
    Histogram hist_tdac0 = FillHistogram(sctdac0, 0, 3, 0.015);
    FitGaus(hist_tdac0, false);
#endif
    Histogram hist_tdac7 ;
    if(delay) hist_tdac7 = FillHistogram(sctdac7, 0, 63, 1);
    else hist_tdac7 = FillHistogram(sctdac7, 0, 3, 0.015);
    FitGaus(hist_tdac7, false);

    //calculate mean threshold:
#ifdef SCANFULLRANGE
    double meanthreshold = (hist_tdac0.GetMean() + hist_tdac7.GetMean()) * 0.5;
#else
    //double meanthreshold = (hist_tdac7.GetMean() - 3 * hist_tdac7.GetSigma());
    double meanthreshold = hist_tdac7.FindMinimum();
    hist_tdac7.PrintToFile("test");
#endif

    //output the measured data:
    std::stringstream sdata("");

#ifdef SCANFULLRANGE
    sdata << "# SCurves for TDAC=0:\n";
    for(auto& it : sctdac0)
    {
        std::stringstream stitle("");
        stitle << "Pixel (" << it.first.column << "|" << it.first.row << ") for TDAC=" << it.first.tdac;

        sdata << it.second.GenerateString(stitle.str()) << std::endl;
    }
    sdata << "\n# The resulting histogram (TDAC=0):\n"
          << hist_tdac0.Print() << std::endl;
#endif

    sdata << "\n# SCurves for TDAC=7:\n";
    for(auto& it : sctdac7)
    {
        std::stringstream stitle("");
        stitle << "Pixel (" << it.first.column << "|" << it.first.row << ") for TDAC=" << it.first.tdac;

        sdata << it.second.GenerateString(stitle.str()) << std::endl;
    }
    sdata << "\n# The resulting histogram (TDAC=7):\n"
          << hist_tdac7.Print() << std::endl;


    sdata << "\n# The resulting mean threshold (in V) is: " << meanthreshold << std::endl;

    Configuration::WriteToFile("Trimming_","_MeanThreshold.dat", sdata.str());

    return meanthreshold;
}

int Trimming::FindVNDACSetting(bool delay, bool *running, Rect area, bool updategui, int sparserows, int random_init, double middlethreshold, double scale)
{
    //set all TDACs to max for the measurement:
    config.GetTDACConfig()->SetAllToValue(7, 7);
    config.WriteRAMMatrix();

    std::vector<int> vndacvalues;
    std::vector<double> means;
    std::vector<double> sigmas;

    std::stringstream sdata("");

    //binary scan of VNDAC:
    int vndac = 0;
    for(int i = /*5*/4; i >= -1 && *running; --i)
    {
        deactivated_pixels.clear(); //list of pixels deactivated during the step

        if(i >= 0)
            vndac |= 1 << i;
        //VNDAC = 0 needs only to be measured if it was an option...
        else if(i < 0 && vndac != 0)
            break;

        std::cout << "     Testing VNDAC = " << vndac << std::endl;
        config.GetATLASPixConfig(Configuration::dac)->SetParameter("vndac", uint(vndac));
        config.SendUpdate(Configuration::dac);

        std::map<Dataset, SCurve> result = MeasureSparseSCurves(running, sparserows, updategui, area, random_init, delay);
        FitSCurves(result, false, 1.0, false);
        Histogram hist;
        if(delay) hist = FillHistogram(result, 0, 63, 1);
        else hist = FillHistogram(result, 0, 3, 0.01);
        FitGaus(hist, false);


        //Write SCurves and distribution to output stream:
        sdata << "# SCurves for VNDAC=" << vndac << std::endl;
        for(auto& it : result)
        {
            std::stringstream stitle("");
            stitle << "Pixel (" << it.first.column << "|" << it.first.row << ") for TDAC=" << it.first.tdac;

            sdata << it.second.GenerateString(stitle.str()) << std::endl;
        }
        sdata << "# Threshold Distribution for VNDAC=" << vndac << std::endl
              << hist.Print() << std::endl;

        vndacvalues.push_back(vndac);
        means.push_back(hist.GetMean());
        sigmas.push_back(hist.GetSigma());

        //if the change was too large, clear the respective bit again:
        if(middlethreshold < hist.GetMean() - scale * hist.GetSigma())
        {
            vndac &= ~(1 << i);
            //reactivate pixels deactivated during too large VNDAC setting:
            for(auto& it : deactivated_pixels)
                config.GetTDACConfig()->SetTDACValue(uint(it.first), uint(it.second), 7);
        }
    }

    deactivated_pixels.clear(); //deactivated pixels are now to be kept turned off

    double distance = 1e10;
    int bestvndac = 63;
    for(unsigned int i = 0; i < means.size(); ++i)
    {
        if(std::abs(means[i] - scale * sigmas[i] -middlethreshold) < distance)
        {
            distance = std::abs(means[i] - scale * sigmas[i] - middlethreshold);
            bestvndac = vndacvalues[i];
        }
    }

    sdata << "# Resulting Value: VNDAC=" << bestvndac << std::endl;

    Configuration::WriteToFile("Trimming_","_VNDAC_Scan.dat", sdata.str());

    return bestvndac;
}

void Trimming::TrimRow(bool delay, bool *running, int row, Rect area, bool updategui, double threshold)
{
    ConfigureInjections(row, true, area.startcol, area.endcol);
    int activepixels = 0;

    //prepare the TDAC values for the current row:
    for(unsigned int i = uint(area.startcol); i <= uint(area.endcol); ++i)
    {
        if(config.GetTDACConfig()->GetTDACValue(i, uint(row)) < 8)
        {
            config.GetTDACConfig()->SetTDACValue(i, uint(row), 7);
            ++activepixels;
        }
    }

    std::vector<std::pair<Dataset, SCurve> > scurves;
    TDAC_Config* tdacs = config.GetTDACConfig();

    //binary search over the TDAC (3bit):
    for(int i = 2; i >= -1; --i)
    {
        int numpixelsneedseven = 0;
        for(unsigned int col = uint(area.startcol); col <= uint(area.endcol); ++col)
        {
            int val = tdacs->GetTDACValue(col, uint(row));
            if(i == -1 && val == 7)
                ++numpixelsneedseven;
            if(val < 8 && i != -1)
                tdacs->SetTDACValue(col, uint(row), uint(val) & ~(1 << i)); //clear one bit
        }
        config.WriteRAMRow(row);

        //do not perform the measurement if it is not necessary. I.e. no pixel will use TDAC=7:
        if(numpixelsneedseven == 0 && i == -1)
            break;

        std::cout << "Cols " << area.startcol << " - " << area.endcol << " ";
        std::map<Dataset, SCurve> newscurves;
        if(delay) newscurves = MeasureSCurvesDelay(running, updategui, activepixels);
        else newscurves = MeasureSCurves(running, updategui, activepixels);

        //remove noise hits:
        auto it = newscurves.begin();
        while(it != newscurves.end())
        {
            if(it->first.row != row || it->first.column < area.startcol || it->first.column > area.endcol)
                it = newscurves.erase(it);
            else
                ++it;
        }

        FitSCurves(newscurves, false, 1.0, false);

        //decorate new scurves with TDAC value used and add them to the list:
        if(newscurves.size() > 0)
        {
            for(auto it = newscurves.begin(); it != newscurves.end(); ++it)
            {
                Dataset dat = it->first;
                dat.tdac = short(config.GetTDACConfig()->GetTDACValue(uint(dat.column), uint(dat.row)));
                scurves.push_back(std::make_pair(dat, it->second));
            }

            for(int col = area.startcol; col <= area.endcol; ++col)
            {
                Dataset dat;
                dat.column = col;
                dat.row    = row;
                auto it = newscurves.find(dat);

                //no hits detected at all, i.e. threshold way too high:
                if(it == newscurves.end())
                    continue;   //do not raise the threshold again

                //test if the fit failed:
                bool failedfit = false;
                if(delay){
                    if(it->second.GetThreshold() < 0 || it->second.GetThreshold() > 63 || it->second.GetNoise() > 5)
                        failedfit = true;
                }
                else {
                if(it->second.GetThreshold() < 0 || it->second.GetThreshold() > 3 || it->second.GetNoise() > 0.5)
                    failedfit = true;
                }

                //for too high threshold for a pixel, lower the threshold again:
                //  (but keep disabled pixels turned off)

                if((failedfit || it->second.GetThreshold() < threshold) && tdacs->GetTDACValue(uint(col), uint(row)) < 8)
                    tdacs->SetTDACValue(uint(col), uint(row), uint(tdacs->GetTDACValue(uint(col), uint(row))) | (1 << i));
            }
        }
    }

    //search for the best fitting TDAC setting for all pixels in the range:
    std::map<Dataset, double> besttdacs;
    for(auto& it : scurves)
    {
        //skip failed fits:
        if(delay==false){if(it.second.GetThreshold() < 0 || it.second.GetThreshold() > 3 || it.second.GetNoise() > 0.5) continue;}
        else{if(it.second.GetThreshold() < 0 || it.second.GetThreshold() > 63 || it.second.GetNoise() > 5) continue;}
        auto pixel = besttdacs.find(it.first);
        if(pixel == besttdacs.end())
            besttdacs.insert(std::make_pair(it.first, it.second.GetThreshold()));
        else if(std::abs(pixel->second - threshold) > std::abs(it.second.GetThreshold() - threshold))
        {
            //replace the entry with the new TDAC setting and threshold:
            Dataset pix = pixel->first;
            pix.tdac = it.first.tdac;
            besttdacs.erase(pixel);
            besttdacs.insert(std::make_pair(pix, it.second.GetThreshold()));
        }
    }

    //set TDACs in the chip to 7 to avoid noise:
    for(unsigned int i = uint(area.startcol); i <= uint(area.endcol); ++i)
        tdacs->SetTDACValue(i, uint(row), 7);
    config.WriteRAMRow(row, true);

    //apply best values:
    for(auto& it : besttdacs)
        tdacs->SetTDACValue(uint(it.first.column), uint(it.first.row), uint(it.first.tdac));

    //Write out the measured data:
    std::stringstream sdata("");
    for(auto& it : scurves)
    {
        std::stringstream stitle("");
        stitle << "Pixel (" << it.first.column << "|" << it.first.row << ")"
              << " for TDAC=" << it.first.tdac;

        sdata << it.second.GenerateString(stitle.str()) << std::endl;
    }

    sdata << "# Chosen TDAC values:\n"
          << "# Column;\tRow;\tTDAC;\tThreshold (in V)\n";
    for(auto& it : besttdacs)
    {
        sdata << "##  " << it.first.column << "\t" << it.first.row << "\t" << it.first.tdac
              << "\t" << it.second << std::endl;
    }

    std::stringstream sout("");
    sout << "_SCurves_row" << row << ".dat";

    Configuration::WriteToFile("Trimming_", sout.str(), sdata.str());
}

bool Trimming::TrimChip(bool delay, bool *running, double* threshold, bool skipvndac, int startcol, int endcol,
                        int startrow, int endrow, bool updategui, void (*showthr)(double, void*))
{
    if(running == nullptr || (*running) == false)
        return false;

    const int sparserows = 372;    //number of rows to pick from the matrix at most for VNDAC scan Ivan was 40
    const double numsigmas = 2.5; //number of sigmas for the VNDAC scaling

    Rect area;
    area.startcol = startcol;
    area.endcol   = endcol;
    area.startrow = startrow;
    area.endrow   = endrow;

    const int stepsperrow = (endcol - startcol + 1 + maxsimultaneouspixels - 1) / maxsimultaneouspixels;

    //calculate number of measurement steps:
    int numsteps = endrow - startrow + 1;
            //number of rows to trim
#ifdef SCANFULLRANGE
    numsteps = numsteps * 4 + ((skipvndac)?2:9) * ((numsteps < sparserows)?numsteps:sparserows);
#else
    numsteps = numsteps * 4 + ((skipvndac)?1:8) * ((numsteps < sparserows)?numsteps:sparserows);
#endif
    //number of rows of SCurves to measure
    numsteps *= stepsperrow; //take into account steps per row
    numsteps *= (interval_end - interval_start) / interval_step + 1;
            //number of measurement points in total for all SCurves
    bool selfblocked = !config.GetProgressBarBlocked() && updategui;
    if(selfblocked)
    {
        config.SetProgressBarMaximum(numsteps);
        config.SetProgressBarValue(0);
        config.BlockProgressBar();
        config.ProcessEvents();
    }

    config.logit("Start Trimming ...");
    Timing::TimePoint start_time = Timing::GetTimeNow();

    //generate start value for random generator to have the same pixels all the time:
    int random_init = int(time(nullptr));

    //create a backup of the TDAC values:
    TDAC_Config* tdac_backup = config.GetTDACConfig();
    TDAC_Config tdacs = *tdac_backup;
    config.SetTDACConfig(&tdacs);
    ASIC_Config2* ap3dac_backup = config.GetATLASPixConfig(Configuration::dac);
    ASIC_Config2 ap3dac = *ap3dac_backup;
    config.SetATLASPixConfig(Configuration::dac, &ap3dac);

    //make sure to measure at a VNDAC setting larger than 0:
    if(ap3dac.GetParameter("vndac") == 0)
    {
        config.logit("VNDAC=0 found. Trimming useless!","",true);
        *running = false;
    }

    config.logit("  determine target threshold");
    double middlethreshold;
    if(threshold == nullptr)
    {
        middlethreshold = FindMeanThreshold(running, area, updategui, sparserows, random_init, delay);
        config.logit(std::string("mean threshold is: " + std::to_string(middlethreshold)));
        if(showthr != nullptr)
            showthr(middlethreshold, nullptr);
    }
    else
        middlethreshold = *threshold;

    config.logit(std::string("used mean threshold is: " + std::to_string(middlethreshold)));
    int vndac = config.GetATLASPixConfig(Configuration::dac)->GetParameter("vndac");
    if(!skipvndac)
    {
        config.logit("  find VNDAC setting");
        vndac = FindVNDACSetting(delay, running, area, updategui, sparserows, random_init, middlethreshold, numsigmas);

        ap3dac.SetParameter("vndac", uint(vndac));
        config.SendUpdate(Configuration::dac);
    }

    Rect partline = area;

    for(int row = startrow; row <= endrow && *running; ++row)
    {
        std::stringstream s("");
        s << "  trimming pixels row " << row;
        config.logit(s.str());
        for(int stcol = startcol; stcol < endcol; stcol += maxsimultaneouspixels)
        {
            int edcol = stcol + maxsimultaneouspixels - 1;
            if(edcol > endcol)
                edcol = endcol;
            partline.startcol = stcol;
            partline.endcol   = edcol;
            TrimRow(delay, running, row, partline, updategui, middlethreshold); //area, updategui, middlethreshold);
        }
    }

    //set all the found values for the TDACs (by now they are all set to 7):
    config.WriteRAMMatrix();

    //write out results:
    std::string outputfile = Configuration::FindFileName("Trimming_","_result.xml");
    tdacs.SaveToXMLFile(outputfile, "", false);

    //for a finished run apply the found settings to the GUI containers:
    if(*running)
    {
        //change only the TDACs that were actually trimmed:
        for(int col = startcol; col <= endcol; ++col)
            for(int row = startrow; row <= endrow; ++row)
                tdac_backup->SetTDACValue(uint(col), uint(row), uint(tdacs.GetTDACValue(uint(col), uint(row))));
        *ap3dac_backup = ap3dac;
    }
    //restore the setting containers of the GUI:
    config.SetTDACConfig(tdac_backup);
    config.WriteRAMMatrix();
    config.SetATLASPixConfig(Configuration::dac, ap3dac_backup);
    config.SendUpdate(Configuration::dac | Configuration::vdac);

    config.logit("Trimming finished");
    Timing::TimePoint end_time = Timing::GetTimeNow();
    config.logit(std::string("  total duration: ") + Timing::TimesToInterval(start_time, end_time));

    if(updategui)
        config.SetProgressBarValue(numsteps);
    if(selfblocked)
        config.ReleaseProgressBar();

    return true;
}
