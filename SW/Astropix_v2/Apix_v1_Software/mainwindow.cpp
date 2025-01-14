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

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
    //ui(new Ui::MainWindow)
{

    // Load UI using Designer Form
    //-------------
    this->ui = new Ui::MainWindow();
    ui->setupUi(this);

    // Create FTDI Class for communication with board
    //-------------
    ftdi = new FTDI();
    ftdi->Close();

    // Create Nexys Class for FGPA Design protocol handling
    //--------------
    nexys = new NexysIO();

    // Other simple UI setup
    //--------------------------

    //-- Default Window size
    this->setWindowState(Qt::WindowMaximized);
    //this->setWindowType(Qt::WindowMaximized);

    //-- Read/Write register Hex value conversion
    this->ui->L_RegisterReadWrite_HexVal->setText(QString());
    connect(this->ui->SB_Register_Value, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[=](int value) {
        QString hexvalue = QString("0x%1").arg(value, 2, 16, QLatin1Char( '0' ));
        this->ui->L_RegisterReadWrite_HexVal->setText(hexvalue);
    });

    //-- Vertical Boxes need to align their compoents to top
    this->ui->scrollAreaVoltageBoardsVBox
            ->layout()
            ->setAlignment(Qt::AlignTop);


    //-------------------------//

    //Register Configurations:
    //----------------------------
    // Bias Block DACS
    //------------------
    asicConfigs.push_back(new ASIC_Config2("DACS"));
    ASIC_Config2 * asic_dacs = asicConfigs.back();

    asic_dacs->AddParameter("q00"        , "0"          ,  0)
                .AddParameter("q01"        , "0"          ,  0)
                .AddParameter("qon0"       , "0"          ,  0)
                .AddParameter("qon1"       , "0"          ,  1)
                .AddParameter("qon2"       , "0"          ,  0)
                .AddParameter("qon3"       , "0"          ,  1)
                .AddParameter("blres"      , "5,4,3,2,1,0",  5)
                .AddParameter("ithres_unused"     , "5,4,3,2,1,0",  0)   //NU
                .AddParameter("vn1"        , "5,4,3,2,1,0", 2)
                .AddParameter("vnfb"       , "5,4,3,2,1,0",  5)
                .AddParameter("vnfoll"     , "5,4,3,2,1,0",  5)
                .AddParameter("vnregc_unused"     , "5,4,3,2,1,0",  0)    //NU
                .AddParameter("vnpdel_unused"     , "5,4,3,2,1,0",  0)
                .AddParameter("vpcomp_unused"     , "5,4,3,2,1,0",  0)
                .AddParameter("vpdac_unused"      , "5,4,3,2,1,0",  0)    //NU
                .AddParameter("vn2"        , "5,4,3,2,1,0",  2)
                .AddParameter("vnfoll2"  , "5,4,3,2,1,0",  5)    //NU
                .AddParameter("vnbias"     , "5,4,3,2,1,0",  5)
                .AddParameter("vpload"     , "5,4,3,2,1,0",  2)
                .AddParameter("vnout_unused"      , "5,4,3,2,1,0",  0)    //NU
                .AddParameter("vncomp"      , "5,4,3,2,1,0", 5)
                .AddParameter("vpfoll"      , "5,4,3,2,1,0", 5)
                .AddParameter("vpdclmu_unusedx"   , "5,4,3,2,1,0", 0)
                .AddParameter("vprec"   , "5,4,3,2,1,0", 5)
                .AddParameter("vnrec"   , "5,4,3,2,1,0", 5)
                //.AddParameter("Recconfigcol0"   , "37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0", 7)
                //34 additional Rec_cols with 38*34 bits
                .AddParameter("Recconfig_unused"   ,1292, 0, 0);

    //-- Digital Config
    //---------------
    asicConfigs.push_back(new ASIC_Config2("Config"));
    ASIC_Config2 * asic_config = asicConfigs.back();
    asic_config->AddParameter("interrupt_pushpull"        , "0" , 0)
                .AddParameter("En_Inj_unused", "17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0"               ,   0)
                .AddParameter("Extrabits_unused"      , "14,13,12,11,10,9,8,7,6,5,4,3,2,1,0"               ,   0)
                .AddParameter("ResetB"      , "0"               ,   0);

                /*.AddParameter("timerendpixel"    , "3,2,1,0"                 ,   0)
                .AddParameter("timerend"         , "3,2,1,0"                 ,   0)
                .AddParameter("ckdivend2"        , "5,4,3,2,1,0"             ,   0)
                .AddParameter("ckdivend"         , "5,4,3,2,1,0"             ,   0);*/





    //configure voltage cards and injection card:
    //  load signals for GECCO board:
    voltageboards.push_back(new VB_Config(3.3, 1.8));
    voltageboards[0]->SetDeviceName("VB1");


   /* voltageboards[1] = VB_Config(3.3, 1.8);
    voltageboards[1].SetDeviceName("VB2");*/
    injection.SetDeviceName("InjectionBoard");

    // First DAC entry is not a DAC
    // it is the Shift Register on the GECCO which controls the Load
    // This DAC value is fixed and should be set to a one hot value
    // The 1 in the value will activate the appropriate Load signal
    // Here 4 and 5 means the 1 will be in position 3 and 4 in the one-hot but, activating Slots 4 and 5
    //voltageboards[0]->addLoadShiftRegister(4);
    //injection.addLoadShiftRegister(5);

    //Add Load DACs for SR
    voltageboards[0]->AddDAC("Ld_7", -1, 0, 1, 0);
    voltageboards[0]->AddDAC("Ld_6", -1, 0, 1, 0);
    voltageboards[0]->AddDAC("Ld_5", -1, 0, 1, 0);
    voltageboards[0]->AddDAC("Ld_4", -1, 0, 1, 0);
    voltageboards[0]->AddDAC("Ld_3", -1, 0, 1, 1);
    voltageboards[0]->AddDAC("Ld_2", -1, 0, 1, 0);
    voltageboards[0]->AddDAC("Ld_1", -1, 0, 1, 0);
    voltageboards[0]->AddDAC("Ld_0", -1, 0, 1, 0);
    injection.AddDAC("Ld_7", -1, 0, 1, 0);
    injection.AddDAC("Ld_6", -1, 0, 1, 0);
    injection.AddDAC("Ld_5", -1, 0, 1, 0);
    injection.AddDAC("Ld_4", -1, 0, 1, 1);
    injection.AddDAC("Ld_3", -1, 0, 1, 0);
    injection.AddDAC("Ld_2", -1, 0, 1, 0);
    injection.AddDAC("Ld_1", -1, 0, 1, 0);
    injection.AddDAC("Ld_0", -1, 0, 1, 0);

    // DAC  Config
    voltageboards[0]->AddDAC("unused", 14, 2, 1, 0);
    voltageboards[0]->AddDAC("unused", 14, 2, 1, 0);
    voltageboards[0]->AddDAC("VCasc2", 14, 2, 1, 0.6);
    voltageboards[0]->AddDAC("BL", 14, 2, 1, 1.0);
    voltageboards[0]->AddDAC("unused", 14, 2, 1, 0);
    voltageboards[0]->AddDAC("unused", 14, 2, 1, 0);
    voltageboards[0]->AddDAC("Vminuspix", 14, 2, 1, 0);
    voltageboards[0]->AddDAC("Vth", 14, 2, 1, 1.02);

    injection.AddDAC("Out1", 14, 2, 1, 0);
    injection.AddDAC("Out2", 14, 2, 1, 0);

    fm_resets.SetFPGAAddress(18);
    fm_resets.SetIdentifier("FMChipResets");
    fm_resets.AddFlag("autoreset_analog", 1, false);
    fm_resets.AddFlag("resetanalogB", 4, false);
    fm_resets.AddFlag("autoreset_digital", 8, false);
    fm_resets.AddFlag("resetdigitalB", 32, false);
    fm_resets.AddFlag("autoreset_combine", 128, false);

    fm_configmode.SetFPGAAddress(19);
    fm_configmode.SetIdentifier("FMChipConfiguration");
    fm_configmode.AddFlag("useSPI"         ,  1, false);
    fm_configmode.AddFlag("bypassCMD"      ,  2, false);
    fm_configmode.AddFlag("EnCDR"          ,  4, false);
    fm_configmode.AddFlag("EnPLL"          ,  8, false);
    fm_configmode.AddFlag("CMD_ClockInvert", 16, false);
    fm_configmode.AddFlag("interfacespeed3", 32, false);
    fm_configmode.AddFlag("TakeFast"       , 64, false);

    fm_workmode.SetFPGAAddress(20);
    fm_workmode.SetIdentifier("FMWorkMode");
    fm_workmode.AddFlag("SelExt"       , 1, false);
    fm_workmode.AddFlag("AlwaysEnableB", 2, false);
    fm_workmode.AddFlag("untrigROEN"   , 4, false);
    fm_workmode.AddFlag("TrigRO_RstN"  , 8, false);

    udpro.SetConfigAddress(33);
    udpro.SetIPAddress(30);
    udpro.SetPortAddress(31);
    udpro.SetSubnetMaskAddress(36);
    udpro.SetMACAddress(32);
    udpro.SetInterPackageDelayAddress(34);
    udpro.SetTimeoutAddress(35);
    if(!udpro.AddressesComplete())
        std::cerr << "UDP Readout Configuration not complete" << std::endl;

    trigger_fm.SetIdentifier("TriggerSettings");
    trigger_fm.SetFPGAAddress(40);
    trigger_fm.AddFlag("posedge", true);
    trigger_fm.AddFlag("ftditrigger", false);
    trigger_fm.AddFlag("source", 4, 3, 0);
    trigger_fm.AddFlag("noise", 32, 3, 0);
    trigger_fm.AddFlag("delay", 1024, 24, 0, false);
    trigger_fm.AddFlag("length", 1024, 24, 0, false);

    //----------------------
    //  GUI Building
    //----------------------
    std::cout << "Setting up GUI DACS" << std::endl;
    std::cout << std::flush;


    // Configuration Tabs
    //------------------------------
    buildASIConfigUI();


    connect(ui->SB_Injection_ClockDiv, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[=](int val) {
       this->injection.SetClockDiv(val);
    });

    connect(ui->SB_Injection_InitDelay, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[=](int val) {
       this->injection.SetInitDelay(val);
    });

    connect(ui->SB_Injection_NumTrains, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[=](int val) {
       this->injection.SetNumPulseSets(val);
    });

    connect(ui->SB_Injection_PulsesInTrain, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[=](int val) {
       this->injection.SetNumPulsesInaSet(val);
    });

    connect(ui->SB_Injection_Period, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[=](int val) {
       this->injection.SetPeriod(val);
    });

    connect(ui->CB_Injection_SyncState, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),[=](int val) {
        switch(val)
        {
        case(0):
            this->injection.SetSynced(0);
            this->injection.SetTSOverflowSync(0);
            break;
        case(1):
            this->injection.SetTSOverflowSync(0);
            this->injection.SetSynced(1);
            break;
        case(2):
            this->injection.SetTSOverflowSync(1);
            this->injection.SetSynced(1);
            break;
        }
    });

    connect(ui->CB_Injection_Output, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),[=](int val) {
        logit(QString().asprintf("Changed Injection Output to %d",val).toStdString(),"Injection");
        if(val >= 0)
            this->injection.SetOutputChannel(1 << val);
        else
           this->injection.SetOutputChannel(0);
    });

    connect(ui->SB_Injection_SignalSize, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),[=](double val) {
       this->injection.SetDAC("Out1", val);
    });

    //VoltageBoards:
    //----------------------
    std::cout << "Setting up Voltage Boards" << std::endl;

    //-- The Voltate UI components are hosted in a VBox inside the scroll area
    //-- Get the Widget and add further components to its layout manager
    QWidget * voltageBoardDACContainer = this->ui->scrollAreaVoltageBoardsVBox;

    //--  actually add the spin boxes,... for the voltage boards:
    unsigned int vbindex = 0;
    unsigned int vboffset = 0;
    for(unsigned int index = 0; index < voltageboards.size(); index++)
    {
        for(unsigned int i = 0; i < voltageboards[index]->GetEntries(); ++i, ++vbindex)
        {
            //Dont create QWidget for DACs with "unused" or "LD_" in Name
            if(voltageboards[index]->GetDACName(i).find("unused") != std::string::npos || voltageboards[index]->GetDACName(i).find("Ld_") != std::string::npos)
            {
                continue;
            }

            // Create HBOX for:
            //   - Spin Box and Label
            //----------------
            QWidget * hboxContainer = new QWidget(); // Widget
            hboxContainer->setLayout(new QHBoxLayout()); // With Hbox
            voltageBoardDACContainer->layout()->addWidget(hboxContainer); // Added to main container

           //-- Add Label
            QLabel* lb = new QLabel();
            hboxContainer->layout()->addWidget(lb);

            lb->setText(voltageboards[index]->GetDACName(i).c_str());
            //lb->setGeometry(10, 10 + 30 * int(vbindex - vboffset), 75,16);

            //-- Add Spin Box
            QDoubleSpinBox* sb = new QDoubleSpinBox();
            hboxContainer->layout()->addWidget(sb);

            sb->setSuffix(" V");
            sb->setDecimals(3);
            sb->setAlignment(Qt::AlignRight);
            sb->setRange(0, 1.8);
            sb->setValue(voltageboards[index]->GetDAC(i));
            sb->setSingleStep(0.05);


            // Update Event
            //-----------
            connect(sb, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),[=](double value) {
               voltageboards[index]->SetDAC(i,value);
                // QString hexvalue = QString("0x%1").arg(value, 2, 16, QLatin1Char( '0' ));
               // this->ui->L_RegisterReadWrite_HexVal->setText(hexvalue);
            });
            // Old -> Use global function
            //connect(sb, SIGNAL(valueChanged(double)), this, SLOT(UpdateFromGUI()));
        }
    }

     std::cout << "Done Setting up Voltage Boards" << std::endl;

    //configure the SPI communication class:
    spiconfig.SetBufferSize(1024);
    spiconfig.SetWriteAddress(23);
    spiconfig.SetReadAddress(24);
    spiconfig.SetClockDivAddress(22);
    spiconfig.SetClockDivider(nexys, 10);
    spiconfig.SetConfigAddress(21);

    //init fast readout flag manager:

    ui->GB_SPI->setVisible(false);
    ui->GB_CMD->setGeometry(ui->GB_SPI->x(), ui->GB_SPI->y(),
                            ui->GB_CMD->width(), ui->GB_CMD->height());
    ui->GB_CMD->setVisible(false);


    // Fill Configuration
    //---------------------
    std::cout << "Setting up Config Object" << std::endl;
    config.SetNexysIO(nexys);
    config.SetATLASPixConfig(Configuration::dac, &atlaspix_dac);
    config.SetATLASPixConfig(Configuration::config, &atlaspix_config);
    config.SetATLASPixConfig(Configuration::vdac, & atlaspix_vdac);
    config.SetATLASPixConfig(Configuration::column, &atlaspix_column);
    config.SetATLASPixConfig(Configuration::row, &atlaspix_row);
    config.SetATLASPixConfig(Configuration::tdac, &atlaspix_tdac);
    config.SetATLASPixConfig(Configuration::srload, &atlaspix_sr_loads);

    //-- Voltage Boards
    std::cout <<  "- Setting up Config Voltage Boards" << std::endl;
    for (VB_Config *   vb  : voltageboards) {
        config.AddVBConfig(vb);
    }
    std::cout <<  "- Done" << std::endl;
    // OLD
    //config.SetVBConfig(0, &(voltageboards[0]));
    //config.SetVBConfig(1, &(voltageboards[1]));

    config.SetPinConfig(Configuration::resets, &fm_resets);
    config.SetPinConfig(Configuration::modeconfig, &fm_configmode);
    config.SetPinConfig(Configuration::modework, &fm_workmode);
    config.SetSPIConfig(&spiconfig);
    config.SetCMDConfig(&cmd);
    config.SetTDACConfig(&tdacs);
    config.SetInjectionConfig(&injection);
#ifdef _useQT_
    config.SetProgressBar(ui->progressBar);
    config.SetLogWindow(ui->Log);
#endif
    //update configuration means:
    on_CB_Configuration_Config_currentIndexChanged(ui->CB_Configuration_Config->currentIndex());


    //Trimming:
    connect(ui->SB_Trim_Start,SIGNAL(valueChanged(int)),this,SLOT(SB_Trim_valueChanged(int)));
    connect(ui->SB_Trim_Step,SIGNAL(valueChanged(int)),this,SLOT(SB_Trim_valueChanged(int)));
    connect(ui->SB_Trim_End,SIGNAL(valueChanged(int)),this,SLOT(SB_Trim_valueChanged(int)));

    trim = new Trimming(config);

    //ToT Calibration:
    connect(ui->SB_ToTCal_Start, SIGNAL(valueChanged(int)), this, SLOT(SB_Trim_valueChanged(int)));
    connect(ui->SB_ToTCal_Step, SIGNAL(valueChanged(int)), this, SLOT(SB_Trim_valueChanged(int)));
    connect(ui->SB_ToTCal_End, SIGNAL(valueChanged(int)), this, SLOT(SB_Trim_valueChanged(int)));

    LoadFileList();
}

void MainWindow::buildASIConfigUI() {


    //-- Each Config set will be a tab in the gui
    //----------
    QTabWidget * asicConfigTabW = this->ui->asicConfigTabWidget;
    asicConfigTabW->clear();
    for (ASIC_Config2* config : this->asicConfigs) {


        std::cout << "-" << config->GetDeviceName() << std::endl;
        std::cout << std::flush;

        //-- Create Vertical Layout container and scroll area
        QScrollArea * tabScrollArea = new QScrollArea();
        QWidget * configContainer = new QWidget();
        configContainer->setLayout(new QVBoxLayout());
        configContainer->layout()->setAlignment(Qt::AlignTop);

        // Insert to tab
        asicConfigTabW->insertTab(-1,tabScrollArea,QString::fromStdString(config->GetDeviceName()));

        //-- Insert entries
        for(unsigned int i = 0; i < config->GetEntries(); ++i) {

            //Dont create HBOX for unused Parameters
            if(config->GetParameterName(i).find("unused") != std::string::npos){
               config->widgets.push_back( nullptr);
                continue;
            }

            // Optional parameters are not set
            if (config->ParameterIsOptional(i)) {
                config->widgets.push_back( nullptr);
                continue;
            }

            //-- Create HBOX to hold parameter GUI
            QWidget * containerWidget = new QWidget();
            QHBoxLayout * hLayout = new QHBoxLayout(containerWidget);
            configContainer->layout()->addWidget(containerWidget);

            //-- Parameter with more than 1 bit get a Slider and Spinbox
            if(config->GetParameterWidth(i) > 1) {

                // Label
                QLabel* lb = new QLabel();
                hLayout->addWidget(lb);

                lb->setText(config->GetParameterName(i).c_str());


                // Slider to change value
                QSlider* sl = new QSlider(Qt::Horizontal);
                hLayout->addWidget(sl);

                sl->setRange(0, (1 << config->GetParameterWidth(i)) - 1);
                sl->setValue(config->GetParameter(i));

                // Spin Box to change value
                QSpinBox* sb = new QSpinBox();
                hLayout->addWidget(sb);
                config->widgets.push_back(sb);

                sb->setRange(0, (1 <<config->GetParameterWidth(i)) - 1);
                sb->setValue(config->GetParameter(i));
                sb->setAlignment(Qt::AlignRight);

                // Events
                // - Slider changes value of Box on release
                // - Box updates using global updateFromGUI() when value changes
                connect(sl,&QSlider::valueChanged, [=](int val){
                    // Update Box if necessary
                    if (sb->value()!=val) {
                         sb->setValue(val);
                    }

                });
                connect(sb, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[=](int val) {
                    // Update Config
                    config->SetParameter(i,val);

                    // Update Slider if not already correct
                    if (sl->value()!=val) {
                        sl->setValue(val);
                    }
                });
                //connect(sb, SIGNAL(valueChanged(int)), this, SLOT(UpdateFromGUI()));

            }
            //-- Parameter with 1 bit are a check box
            else {

                // Checkbox
                QCheckBox* cb = new QCheckBox();
                hLayout->addWidget(cb);
                config->widgets.push_back(cb);

                cb->setText(config->GetParameterName(i).c_str());
                cb->setChecked(config->GetParameter(i) != 0);

                connect(cb, &QCheckBox::stateChanged,[=](int val){
                    //logit(QString().asprintf("Changing Param %d to %d",i, val).toStdString());
                    config->SetParameter(i,val == Qt::Checked? 1 : 0);
                });

                /*connect(config,&ASIC_Config2::valueChanged,[=](unsigned int index,int val) {
                    logit(QString().asprintf("Param changed, updating ui %d to %d",i, val).toStdString());
                    cb->setChecked(val != 0);
                });*/
               // connect(cb, SIGNAL(stateChanged(int)), this, SLOT(UpdateFromGUI()));

            }

        }

        // Add Config Widget to scroll area here not before otherwise it is not drawn correctly
        tabScrollArea->setWidget(configContainer);

    }


}

MainWindow::~MainWindow()
{
    delete ui;

    delete nexys;
    delete ftdi;
}

std::string MainWindow::WriteToFile(std::string filename, std::string data)
{
    std::fstream f;
    f.open(filename.c_str(), std::ios::out | std::ios::app | std::ios::binary);
    if(!f.is_open())
        return "";

    f << data;
    f.flush();
    f.close();

    return filename;
}

void MainWindow::ProcessEvents()
{
    QApplication::processEvents();
}

void MainWindow::setWaiter(WaitFunction newwaiter)
{
    this->waiter = newwaiter;
    config.setWaiter(waiter);
    trim->GetConfig()->setWaiter(waiter);
}

void MainWindow::logit(std::string logstream, std::string header)
{
    static std::string localheader = "";
    static int linecounter = 0;

    std::cout << logstream << std::endl;
    QString buffer= QString::fromStdString(logstream);
    if(header.compare(localheader) != 0)
    {
        localheader = header;
        if(header != "")
            ui->Log->append(QString::fromStdString(header));
    }

    //security check to prevent crashes due to memory issue in QTextEdit
    if(linecounter > 5000)
    {
        ui->Log->clear();
        linecounter = 0;
    }

    ui->Log->append(buffer);
    QTextCursor c =  ui->Log->textCursor();
    c.movePosition(QTextCursor::End);
    ui->Log->setTextCursor(c);

    ++linecounter;
}

void MainWindow::UpdateInjectionBoardFromGUI() {

}

void MainWindow::UpdateFromGUI()
{
    QObject* obj = sender();
    bool update = ui->CB_Autoupdate->isChecked();
    ASIC_Config2* config = nullptr;
    VB_Config*    config_vb = nullptr;
    Injection_Config* config_inj = nullptr;

    ASIC_Config2* configs[6] = {&atlaspix_dac, &atlaspix_config, &atlaspix_vdac,
                                &atlaspix_tdac, &atlaspix_row, &atlaspix_column};

    //ATLSPix3:
    bool found = false;
    for(int i = 0; i < 6; ++i)
    {
        if(obj->parent() == dac_gbs[static_cast<unsigned int>(i)])
        {
            config = configs[i];
            found = true;
        }
    }

    //Injection:
    /*if(found);
    else if(obj->parent() == ui->L_ParentAnchorInjection->parent())
        config_inj = &injection;
    else if(obj->parent() == dac_gbs[6])
        config_vb = &(voltageboards[0]);
    else if(obj->parent() == ui->L_ParentAnchorFM->parent());
    else
        logit("some parts are not implemented yet");*/

    if(config != nullptr)
    {
        for(unsigned int i = 0; i < dac_labels.size(); ++i)
        {
            if(obj == dac_sliders[i] && dac_sliders[i]->value() != dac_spinboxes[i]->value())
            {
                dac_spinboxes[i]->setValue(dac_sliders[i]->value());
                config->SetParameter(dac_labels[i]->text().toStdString(), static_cast<unsigned int>(dac_sliders[i]->value()));
                if(!update)
                {
                    if(ui->Update->text().toStdString().find("(!)") == std::string::npos)
                        ui->Update->setText(ui->Update->text() + " (!)");
                }
                else
                    on_Update_clicked();
            }
            else if(obj == dac_spinboxes[i] && dac_sliders[i]->value() != dac_spinboxes[i]->value())
            {
                dac_sliders[i]->setValue(dac_spinboxes[i]->value());
                config->SetParameter(dac_labels[i]->text().toStdString(), static_cast<unsigned int>(dac_spinboxes[i]->value()));
                if(!update)
                {
                    if(ui->Update->text().toStdString().find("(!)") == std::string::npos)
                        ui->Update->setText(ui->Update->text() + " (!)");
                }
                else
                    on_Update_clicked();
            }
            else if(obj == dac_checkboxes[i])
            {
                config->SetParameter(dac_labels[i]->text().toStdString(), (dac_checkboxes[i]->isChecked())?1:0);
                if(!update)
                {
                    if(ui->Update->text().toStdString().find("(!)") == std::string::npos)
                        ui->Update->setText(ui->Update->text() + " (!)");
                }
                else
                    on_Update_clicked();
            }
        }
    }
    else if(config_inj != nullptr)
    {
        if(obj == ui->SB_Injection_ClockDiv)
            config_inj->SetClockDiv(static_cast<unsigned int>(ui->SB_Injection_ClockDiv->value()));
        else if(obj == ui->SB_Injection_InitDelay)
            config_inj->SetInitDelay(static_cast<unsigned int>(ui->SB_Injection_InitDelay->value()));
        else if(obj == ui->SB_Injection_NumTrains)
            config_inj->SetNumPulseSets(static_cast<unsigned int>(ui->SB_Injection_NumTrains->value()));
        else if(obj == ui->SB_Injection_PulsesInTrain)
            config_inj->SetNumPulsesInaSet(static_cast<unsigned int>(ui->SB_Injection_PulsesInTrain->value()));
        else if(obj == ui->SB_Injection_Period)
            config_inj->SetPeriod(static_cast<unsigned int>(ui->SB_Injection_Period->value()));
        else if(obj == ui->CB_Injection_SyncState)
        {
            switch(ui->CB_Injection_SyncState->currentIndex())
            {
            case(0):
                config_inj->SetSynced(0);
                config_inj->SetTSOverflowSync(0);
                break;
            case(1):
                config_inj->SetTSOverflowSync(0);
                config_inj->SetSynced(1);
                break;
            case(2):
                config_inj->SetTSOverflowSync(1);
                config_inj->SetSynced(1);
                break;
            }
        }
        else if(obj == ui->CB_Injection_Output)
        {
            if(ui->CB_Injection_Output->currentIndex() >= 0)
                config_inj->SetOutputChannel(1 << ui->CB_Injection_Output->currentIndex());
            else
                config_inj->SetOutputChannel(0);
        }
        else if(obj == ui->SB_Injection_SignalSize)
            config_inj->SetDAC("Out1", ui->SB_Injection_SignalSize->value());

        //add a sign that something changed and the configuration should be updated:
        if(ui->B_Injection_StartStop->text().toStdString().find('!') == std::string::npos)
            ui->B_Injection_StartStop->setText(ui->B_Injection_StartStop->text() + " (!)");
    }
    else if(config_vb != nullptr)
    {

    }
}

void MainWindow::UpdateFromConfig()
{
    ASIC_Config2* configs[6] = {&atlaspix_dac, &atlaspix_config, &atlaspix_vdac,
                                &atlaspix_tdac, &atlaspix_row, &atlaspix_column};

    int fail = 0;

    for(unsigned int index = 0; index < dac_labels.size(); ++index)
    {
        int sr = -1;
        for(int i = 0; i < 6; ++i)
            if(dac_labels[index]->parentWidget() == dac_gbs[static_cast<unsigned int>(i)])
                sr = i;
        if(sr < 0 || sr > 5)
        {
            ++fail;
            logit("Error: could not identify parent of \"" + dac_labels[index]->text().toStdString() + "\"");
            continue;
        }

        if(dac_sliders[index] != nullptr)
        {
            int value = configs[sr]->GetParameter(dac_labels[index]->text().toStdString());
            if(value >= 0)
                dac_sliders[index]->setValue(value);
            else
            {
                logit("Error: Parameter \"" + dac_labels[index]->text().toStdString() + "\" not found on GUI");
                ++fail;
            }
        }
        else if(dac_checkboxes[index] != nullptr)
        {
            int value = configs[sr]->GetParameter(dac_labels[index]->text().toStdString());
            if(value >= 0)
                dac_checkboxes[index]->setChecked(value != 0);
            else
                ++fail;
        }
    }

    //Injection:
    ui->SB_Injection_ClockDiv->setValue(int(injection.GetClockDiv()));
    ui->SB_Injection_InitDelay->setValue(int(injection.GetInitDelay()));
    ui->SB_Injection_NumTrains->setValue(int(injection.GetNumPulseSets()));
    ui->SB_Injection_PulsesInTrain->setValue(int(injection.GetNumPulsesInaSet()));
    ui->SB_Injection_Period->setValue(int(injection.GetPeriod()));
    //ui->SL_Injection_Synced->setValue(int(injection.GetSynced()));
    if(!injection.GetSynced())
        ui->CB_Injection_SyncState->setCurrentIndex(0);
    else if(!injection.GetTSOverflowSync())
        ui->CB_Injection_SyncState->setCurrentIndex(1);
    else
        ui->CB_Injection_SyncState->setCurrentIndex(2);
    ui->SB_Injection_SignalSize->setValue(injection.GetDAC("Out1"));
    switch(injection.GetOutputChannel() & (Injection_Config::GECCO + Injection_Config::ATLASPix3))
    {
    case(0):
        ui->CB_Injection_Output->setCurrentIndex(-1);
        break;
    case(Injection_Config::GECCO):
        ui->CB_Injection_Output->setCurrentIndex(0);
        break;
    case(Injection_Config::ATLASPix3):
        ui->CB_Injection_Output->setCurrentIndex(1);
        break;
    default:
        break;
    }

    // Voltage Boards:
    /*for(unsigned int i = 0; i < vb_label.size(); ++i)
    {
        double value1 = voltageboards[0].GetDAC(vb_label[i]->text().toStdString());
        double value2 = voltageboards[1].GetDAC(vb_label[i]->text().toStdString());
        if(value1 >= 0)
            vb_spinbox[i]->setValue(value1);
        else if(value2 >= 0)
            vb_spinbox[i]->setValue(value2);
        else
            ++fail;
    }*/

    //Pin Configuration from FPGA:
    for(auto& it : fm_checkboxes)
    {
        int value = 0;
        std::string text = it->text().toStdString();
        value = fm_resets.GetFlag(text);
        if(value < 0)
            value = fm_configmode.GetFlag(text);
        if(value < 0)
            value = fm_workmode.GetFlag(text);
        if(value >= 0)
            it->setChecked(value != 0);
        else
            ++fail;
    }

    //SPI configuration:
    ui->CB_SPI_Enable->setChecked(spiconfig.GetSPIEnable());
    ui->CB_SPI_ReadbackEn->setChecked(spiconfig.GetReadBackEnable());
    ui->SB_SPI_ClockDiv->setValue(spiconfig.GetClockDivider());

    //update the Col/Row/TDAC tab:
   /* SB_Config_Injection_valueChanged(0);
    on_SB_Config_AmpOut_HB_Col_valueChanged(ui->SB_Config_AmpOut_HB_Col->value());
    SB_TDAC_Address_valueChanged(0);*/

    //update trigger settings:
    ui->CB_Trig_PosEdge->setChecked(trigger_fm.GetFlag("posedge"));
    ui->CB_Trig_Source->setCurrentIndex(trigger_fm.GetFlag("source"));
    ui->CB_Trig_NoiseReject->setCurrentIndex(trigger_fm.GetFlag("noise"));
    ui->SB_Trig_PulseDelay->setValue(trigger_fm.GetFlag("delay"));
    ui->SB_Trig_PulseLength->setValue(trigger_fm.GetFlag("length"));

    //UDP configuration:
    ui->CB_UDP_Reset->setChecked(udpro.GetUDPReset());
    ui->CB_UDP_Reset_Sender->setChecked(udpro.GetSenderReset());
    ui->CB_UDP_Reset_FIFO->setChecked(udpro.GetFIFOReset());
    ui->CB_UDP_Debug->setChecked(udpro.GetDebugMode());
    ui->SB_UDP_InterpackageDelay->setValue(udpro.GetInterPackageDelay());
    ui->SB_UDP_Timeout->setValue(udpro.GetTimeout());
    ui->LE_UDP_IP->setText(udpro.GetIP().c_str());
    ui->SB_UDP_Port->setValue(udpro.GetFPGAPort());
    ui->LE_UDP_SubnetMask->setText(udpro.GetSubnetMask().c_str());
    ui->LE_UDP_MAC->setText(udpro.GetMAC().c_str());

    if(fail > 0)
        logit("Error: Failed to find " + QString::number(fail).toStdString() + " parameters");
    else
    {
        //mark the update buttons as not updated:
        if(ui->Update->text().toStdString().find("(!)") == std::string::npos)
            ui->Update->setText(ui->Update->text() + " (!)");
        if(ui->B_WriteVoltageBoards->text().toStdString().find("(!)") == std::string::npos)
            ui->B_WriteVoltageBoards->setText(ui->B_WriteVoltageBoards->text() + " (!)");
        if(ui->B_Injection_StartStop->text().toStdString().find("(!)") == std::string::npos)
            ui->B_Injection_StartStop->setText(ui->B_Injection_StartStop->text() + " (!)");
       // if(ui->B_PinConfig_Update->text().toStdString().find("(!)") == std::string::npos)
       //     ui->B_PinConfig_Update->setText(ui->B_PinConfig_Update->text() + " (!)");
        if(ui->B_UDP_Update->text().toStdString().find("(!)") == std::string::npos)
            ui->B_UDP_Update->setText(ui->B_UDP_Update->text() + " (!)");

        logit("Update from config successful");
    }
}

void MainWindow::UpdateInjectionGUI(bool start)
{
    //Injection:
    ui->SB_Injection_ClockDiv->setValue(int(injection.GetClockDiv()));
    ui->SB_Injection_InitDelay->setValue(int(injection.GetInitDelay()));
    ui->SB_Injection_NumTrains->setValue(int(injection.GetNumPulseSets()));
    ui->SB_Injection_PulsesInTrain->setValue(int(injection.GetNumPulsesInaSet()));
    ui->SB_Injection_Period->setValue(int(injection.GetPeriod()));
    //ui->SL_Injection_Synced->setValue(int(injection.GetSynced()));

    //Injection mode async
    ui->CB_Injection_SyncState->setCurrentIndex(0);

    //Injection source injectionboard
    ui->SB_Injection_SignalSize->setValue(injection.GetDAC("Out1"));
    ui->CB_Injection_Output->setCurrentIndex(1);
    ui->CB_Injection_Output->setCurrentIndex(0);

    connect(ui->CB_Injection_Output, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),[=](int val) {
        logit(QString().asprintf("Changed Injection Output to %d",val).toStdString(),"Injection");
        if(val >= 0)
            this->injection.SetOutputChannel(1 << val);
        else
           this->injection.SetOutputChannel(0);
    });

    if(ui->B_Injection_StartStop->text().toStdString().find("Start Injections") != std::string::npos)
    {
        //Click button once to start
        if(start){
            this->on_B_Injection_StartStop_clicked();
        }
    }
    else
    {
        if(start){
            //Click button twice to restart
            this->on_B_Injection_StartStop_clicked();
            this->on_B_Injection_StartStop_clicked();
        }
        else{
            this->on_B_Injection_StartStop_clicked();
        }
    }

}


bool MainWindow::ConfigureInjections(bool flush)
{
    if(!nexys->is_open())
        return false;

    bool status = true;

    //signal strength:
    status &= nexys->WritePCB(NexysIO::FPGA_VOLTAGEBOARD_CONFIG, injection.GenerateBitVector(), 8);

    //sync and output settings:
    status &= nexys->Write(Injection_Config::OutputSyncAddress,
                           byte(injection.GetSynced() * Injection_Config::SyncFlag
                           + (injection.GetOutputChannel() & ~Injection_Config::SyncFlag)
                                + injection.GetTSOverflowSync() * 8));

    //clock divider:
    int clockdiv = int(injection.GetClockDiv());
    if(clockdiv > 0)
        --clockdiv;

    //timing, number of pulse sets:
    nexys->PatGen(int(injection.GetPeriod()), int(injection.GetNumPulseSets()),
                                clockdiv, int(injection.GetInitDelay()));

    //pulses in a set:
    status &= nexys->PatGenWrite(7, static_cast<unsigned char>(injection.GetNumPulsesInaSet()));

    if(flush)
        nexys->Flush();

    return status;
}

bool MainWindow::StartInjections(bool flush)
{
    nexys->PatGenSuspend(1);
    nexys->PatGenReset(1);
    nexys->PatGenReset(0);
    nexys->PatGenSuspend(0);
    if(flush)
        return nexys->Flush();
    else
        return true;
}

bool MainWindow::RestartInjections(bool flush)
{
    nexys->PatGenReset(1);
    nexys->PatGenReset(0);
    nexys->PatGenSuspend(0);
    if(flush)
        return nexys->Flush();
    else
        return true;
}

bool MainWindow::StopInjections(bool flush)
{
    nexys->PatGenSuspend(true);
    nexys->PatGenReset(true);
    if(flush)
        return nexys->Flush();
    else
        return true;
}

std::vector<Dataset> MainWindow::ReadAllHits(bool* running, int maxemptyreads, int readsize)
{
    return config.ReadoutAll(running, true, 0, maxemptyreads, readsize);
}

void MainWindow::on_SearchDevices_clicked()
{

    if(ftdi != nullptr)
        ftdi->Close();
    if(nexys != nullptr)
        nexys->Close();
    ui->Devices->clear();
    FT_STATUS ftStatus;
    FT_HANDLE ftHandleTemp;
    DWORD numDevs;
    DWORD Flags;
    DWORD ID;
    DWORD Type;
    DWORD LocId;
    char SerialNumber[16];
    char Description[64];


    // create the device information list
    ftStatus = FT_CreateDeviceInfoList(&numDevs);
    if (ftStatus == FT_OK && numDevs <= 100)
        logit("Number of devices: " + QString::number(numDevs).toStdString());
    else if(numDevs <= 100)
    {
        logit("Could not get the number of devices!");
        return;
    }

    unsigned int maxdevices = numDevs;
    if(numDevs > 100)
    {
        std::cout << "Number of Devices too large. Expecting failure." << std::endl;
        maxdevices = 100;
    }

    int devcounter = 0;
    //else
    {
        std::string Descrip;
        for(unsigned int i = 0; i < numDevs && i < maxdevices; i++)
        {
            // get information for all devices
            ftStatus = FT_GetDeviceInfoDetail(i, &Flags, &Type, &ID, &LocId, SerialNumber, Description, &ftHandleTemp);
            if (ftStatus == FT_OK)
            {
                std::cout << "Dev " << i << ":" << std::endl
                          << " Flags  =" << Flags << std::endl
                          << " Type = " << Type << std::endl
                          << " ID = " << ID << std::endl
                          << " LocId = " << LocId << std::endl
                          << " SerialNumber = " << SerialNumber << std::endl
                          << " Description = " << Description << std::endl
                          << " ftHandle = " << ftHandleTemp << std::endl;
                Descrip = QString::number(i).toStdString();
                Descrip.append(" ");
                Descrip.append(std::string(Description));
                ui->Devices->addItem(QString::fromStdString(Descrip));

                ui->OpenDevice->setEnabled(true);
                ++devcounter;
            }
        }
    }
    if (ui->Devices->itemText(0) == "")
    {
        ui->Devices->addItem("No device detected");
        ui->OpenDevice->setEnabled(false);
    }

    if(maxdevices == 100)
        logit("Number of devices: " + QString::number(devcounter).toStdString());

    return;
}

void MainWindow::on_OpenDevice_clicked()
{
    FT_HANDLE ftHandleTemp = nullptr;
    bool open = false;
    if (ui->Devices->currentText().compare("No device detected") != 0)
    {
        ftdi->Close();
        //ftdi->purge();
        open = ftdi->Open(QString::fromStdString(ui->Devices->currentText().toStdString().substr(0,1)).toInt());
        if(!open)
            logit("Board could not be initialized.");
        else
            logit("Board successfully initialized.");

        //Select FTDI synchronous mode for NexysVideo:
        ftdi->setBitMode(0xFF,0x00);
        ftdi->setBitMode(0xFF,0x40);
        FT_SetLatencyTimer(ftHandleTemp,2);
        FT_SetUSBParameters(ftHandleTemp, 64000, 64000); //0x10000,0x10000);
        ftdihandle = &ftHandleTemp;
        nexys->initializeFtdi(ftdi);

        logit("Channel " + ui->Devices->currentText().toStdString().substr(0,1) + " opened.");

        ui->SearchDevices->setEnabled(false);

        spiconfig.SetSPIEnable(nexys, false);  //make sure that the SPI module is turned off after connecting
    }
    else
        logit("No Device selected");
}

void MainWindow::on_CloseDevice_clicked()
{
    if(nexys->is_open())
    {
        nexys->Close();
        FT_Close(*ftdihandle);
    }

    logit("Device closed.");
    ui->SearchDevices->setEnabled(true);
    return;
}

void MainWindow::on_Update_clicked()
{
    // Nexys Not Open
    if(!nexys->is_open()) {
        logit("Nexys Board not opened!");
        return;
    }
    if(!nexys->is_programmed()) {
        logit("Nexys Board not programmed!");
        return;
    }

    // Update Counter displaying number of updates performed
    int count = ui->UpdateCounterEdit->text().toInt();
    ui->UpdateCounterEdit->setText(QString::number(++count));

    //remove the not updated marker:
    if(ui->Update->text().toStdString().find("(!)") != std::string::npos)
    {
        std::string caption = ui->Update->text().toStdString();
        caption = caption.substr(0, caption.length() - 4);
        ui->Update->setText(caption.c_str());
    }

    // Send Update of selected DAC parts
    //-----------
    bool result = config.SendASICConfigsViaSR(this->asicConfigs,true);

    /*int configflags = 0;
    configflags += (ui->CB_Config_DAC->isChecked())?Configuration::dac:0;
    configflags += (ui->CB_Config_Config->isChecked())?Configuration::config:0;
    configflags += (ui->CB_Config_VDAC->isChecked())?Configuration::vdac:0;
    configflags += (ui->CB_Config_ColRow->isChecked())?Configuration::colrow:0;
*/
    if(!result)
        logit("WriteASIC failed!");
    else
        logit("WriteASIC successful");
}

void MainWindow::on_B_Config_Load_clicked()
{
    bool autoconfig = ui->CB_Autoupdate->isChecked();
    ui->CB_Autoupdate->setChecked(false);

    //loading chip configuration:
    std::string text = ui->CB_SaveLoad_Filename->currentText().toStdString();
    if(text != "")
    {
        bool compressed = (text.find(".xml") == std::string::npos);
        tinyxml2::XMLError error = atlaspix_config_manager.LoadFile(text, compressed);

        if(error == tinyxml2::XML_NO_ERROR)
        {
            logit("Configuration loaded from \"" + text + "\"");

            //atlaspix3 configurations:
            // Add Configs
            for(auto it : this->asicConfigs) {

               auto savedConfigNode =  atlaspix_config_manager.GetConfigNode(it->GetDeviceName());
               if (savedConfigNode!=nullptr) {
                    it->LoadFromXMLElement(savedConfigNode);
               }
                //atlaspix_config_manager.AddConfig(*it, it->GetDeviceName());
            }

            //voltage boards:
            tinyxml2::XMLElement* node;

            //injection (card):
            node = atlaspix_config_manager.GetConfigTree("ShiftRegister", injection.GetDeviceName());
            if(node != nullptr)
                injection.LoadFromXMLElement(node);

            //Pin Configuration:
            node = atlaspix_config_manager.GetConfigTree("FlagManager", fm_resets.GetIdentifier());
            if(node != nullptr)
                fm_resets.LoadFromXMLElement(node);
            node = atlaspix_config_manager.GetConfigTree("FlagManager", fm_configmode.GetIdentifier());
            if(node != nullptr)
                fm_configmode.LoadFromXMLElement(node);
            node = atlaspix_config_manager.GetConfigTree("FlagManager", fm_workmode.GetIdentifier());
            if(node != nullptr)
                fm_workmode.LoadFromXMLElement(node);

            //SPI configuration:
            node = atlaspix_config_manager.GetConfigTree("SPIConfig", spiconfig.GetIdentifier());
            if(node != nullptr)
                spiconfig.LoadFromXMLElement(node);

            //Trigger Settings:
            node = atlaspix_config_manager.GetConfigTree("FlagManager", trigger_fm.GetIdentifier());
            if(node != nullptr)
                trigger_fm.LoadFromXMLElement(node);

            //UDP configuration:
            node = atlaspix_config_manager.GetConfigTree("UDPConfig", udpro.GetIdentifier());
            if(node != nullptr)
                udpro.LoadFromXMLElement(node);

            UpdateFromConfig();

            int position = ui->CB_SaveLoad_Filename->findText(text.c_str());
            if(position == -1)
            {
                ui->CB_SaveLoad_Filename->insertItem(0,text.c_str());
                ui->CB_SaveLoad_Filename->setCurrentIndex(0);
            }
            else
            {
                ui->CB_SaveLoad_Filename->removeItem(position);
                ui->CB_SaveLoad_Filename->insertItem(0, text.c_str());
                ui->CB_SaveLoad_Filename->setCurrentIndex(0);
            }
        }
        else
        {
            logit("Error loading configuration from \"" + text + "\":");
            switch(error)
            {
            case(tinyxml2::XML_ERROR_FILE_NOT_FOUND):
                logit("  File not Found");
                break;
            case(tinyxml2::XML_ERROR_EMPTY_DOCUMENT):
                logit("  Empty Document");
                break;
            case(tinyxml2::XML_ERROR_PARSING):
                logit("   Parsing Error");
                break;
            default:
                logit("   Error Code: " + QString::number(error).toStdString());
                break;
            }
        }
    }

    //TDAC loading:
    text = ui->CB_SaveLoad_Filename_TDACs->currentText().toStdString();
    if(text != "")
    {
        bool compressed = (text.find(".xml") == std::string::npos);
        tinyxml2::XMLError error = tdacs.LoadFromXMLFile(text, compressed);


        logit("TDAC loading from \"" + text + "\" ...");
        switch(error)
        {
        case(tinyxml2::XML_NO_ERROR):
            logit("  Successful");
            SB_TDAC_Address_valueChanged(0); //update shown number for TDACs
            {
                int position = ui->CB_SaveLoad_Filename_TDACs->findText(text.c_str());
                if(position == -1)
                {
                    ui->CB_SaveLoad_Filename_TDACs->insertItem(0, text.c_str());
                    ui->CB_SaveLoad_Filename_TDACs->setCurrentIndex(0);
                }
                else
                {
                    ui->CB_SaveLoad_Filename_TDACs->removeItem(position);
                    ui->CB_SaveLoad_Filename_TDACs->insertItem(0, text.c_str());
                    ui->CB_SaveLoad_Filename_TDACs->setCurrentIndex(0);
                }
            }
            break;
        case(tinyxml2::XML_ERROR_FILE_NOT_FOUND):
            logit("  File not Found");
            break;
        case(tinyxml2::XML_ERROR_EMPTY_DOCUMENT):
            logit("  Empty Document");
            break;
        case(tinyxml2::XML_ERROR_PARSING):
            logit("   Parsing Error");
            break;
        default:
            logit("   Error Code: " + QString::number(error).toStdString());
            break;
        }
    }

    SaveFileList();

    ui->CB_Autoupdate->setChecked(autoconfig);
}

void MainWindow::on_B_Config_Save_clicked()
{
    //Chip configuration:
    std::string text = ui->CB_SaveLoad_Filename->currentText().toStdString();
    if(text != "")
    {
        atlaspix_config_manager.ClearFile();

        // Add Configs
        for(auto it : this->asicConfigs) {
            atlaspix_config_manager.AddConfig(*it, it->GetDeviceName());
        }

        for (auto vb : voltageboards) {
            atlaspix_config_manager.AddConfigTree(
                                vb->SaveToXMLElement(*atlaspix_config_manager.GetParentDocument()));
        }
        //atlaspix_config_manager
        atlaspix_config_manager.AddConfigTree(injection.SaveToXMLElement(*atlaspix_config_manager.GetParentDocument()));

        atlaspix_config_manager.AddConfigTree(fm_resets.SaveToXMLElement(*atlaspix_config_manager.GetParentDocument()));
        atlaspix_config_manager.AddConfigTree(fm_configmode.SaveToXMLElement(*atlaspix_config_manager.GetParentDocument()));
        atlaspix_config_manager.AddConfigTree(fm_workmode.SaveToXMLElement(*atlaspix_config_manager.GetParentDocument()));

        atlaspix_config_manager.AddConfigTree(spiconfig.SaveToXMLElement(*atlaspix_config_manager.GetParentDocument()));

        atlaspix_config_manager.AddConfigTree(trigger_fm.SaveToXMLElement(*atlaspix_config_manager.GetParentDocument()));

        atlaspix_config_manager.AddConfigTree(udpro.SaveToXMLElement(*atlaspix_config_manager.GetParentDocument()));

        std::string comp = text.substr(text.rfind('.'));
        bool compressed = (comp.compare(".xml") != 0);
        tinyxml2::XMLError error = atlaspix_config_manager.SaveToFile(text, compressed);



        if(error == tinyxml2::XML_NO_ERROR)
        {
            logit("Configuration saved to \"" + text + "\"");
            int position = ui->CB_SaveLoad_Filename->findText(text.c_str());
            if(position == -1)
            {
                ui->CB_SaveLoad_Filename->insertItem(0, text.c_str());
                ui->CB_SaveLoad_Filename->setCurrentIndex(0);
            }
            else
            {
                ui->CB_SaveLoad_Filename->removeItem(position);
                ui->CB_SaveLoad_Filename->insertItem(0, text.c_str());
                ui->CB_SaveLoad_Filename->setCurrentIndex(0);
            }
        }
        else
            logit("Error saving configuration to \"" + text + "\"");
     }

    //TDAC values:
    text = ui->CB_SaveLoad_Filename_TDACs->currentText().toStdString();
    if(text != "")
    {
        std::string comp = text.substr(text.rfind('.'));
        bool compressed = (comp.compare(".xml") != 0);
        tinyxml2::XMLError error = tdacs.SaveToXMLFile(text, "", compressed);

        if(error == tinyxml2::XML_NO_ERROR)
        {
            logit("TDAC Configuration saved to \"" + text + "\"");
            int position = ui->CB_SaveLoad_Filename_TDACs->findText(text.c_str());
            if(position == -1)
            {
                ui->CB_SaveLoad_Filename_TDACs->insertItem(0, text.c_str());
                ui->CB_SaveLoad_Filename_TDACs->setCurrentIndex(0);
            }
            else
            {
                ui->CB_SaveLoad_Filename_TDACs->removeItem(position);
                ui->CB_SaveLoad_Filename_TDACs->insertItem(0, text.c_str());
                ui->CB_SaveLoad_Filename_TDACs->setCurrentIndex(0);
            }
        }
        else
            logit("Error saving TDAC configuration to \"" + text + "\"");
    }

    SaveFileList();
}

void MainWindow::on_B_WriteVoltageBoards_clicked()
{
    // Nexys Not Open
    if(!nexys->is_open()) {
        logit("Nexys Board not opened!");
        return;
    }
    if(!nexys->is_programmed()) {
        logit("Nexys Board not programmed!");
        return;
    }


    //remove not updated marker:
    if(ui->B_WriteVoltageBoards->text().toStdString().find("(!)") != std::string::npos)
    {
        std::string caption = ui->B_WriteVoltageBoards->text().toStdString();
        caption = caption.substr(0, caption.length() - 4);
        ui->B_WriteVoltageBoards->setText(caption.c_str());
    }


    logit("Writing VoltageBoards ...");
    ui->B_WriteVoltageBoards->setEnabled(false);


        if(config.ConfigureVoltageBoards(true))
            logit("  successful.");
        else
            logit("  failed.");


    ui->B_WriteVoltageBoards->setEnabled(true);

}

void MainWindow::on_B_Register_Read_clicked()
{
    if(!nexys->is_open())
    {
        logit("No open Connection to Nexys!");
        return;
    }
    else
    {
        std::string answer = nexys->Read(static_cast<unsigned char>(ui->SB_Register_Address->value()), 1);

        ui->SB_Register_Value->setValue(int(answer.c_str()[0]) & 255);

        std::stringstream s("");
        s << "Value of Register " << ui->SB_Register_Address->value() << " is: " << (int(answer.c_str()[0]) & 255);
        logit(s.str());
    }

}

void MainWindow::on_B_Register_Write_clicked()
{
    if(!nexys->is_open())
    {
        logit("No open Connection to Nexys!");
        return;
    }
    else
    {
        if(!nexys->Write(static_cast<unsigned char>(ui->SB_Register_Address->value()), byte(ui->SB_Register_Value->value())))
            logit("Sending Write Command failed!");
        else
        {
            std::stringstream s("");
            s << "Set Register " << ui->SB_Register_Address->value() << " to: " << ui->SB_Register_Value->value();
            logit(s.str());
        }
    }
}

void MainWindow::SetSpinBoxMinMax(QDoubleSpinBox* spinstart, QDoubleSpinBox* spinstop, double min, double max)
{
    if(spinstart != nullptr)
    {
        spinstart->setMinimum(min);
        spinstart->setMaximum(max);
        if(spinstart->value() > max)
            spinstart->setValue(max);
        else if(spinstart->value() < min)
            spinstart->setValue(min);
    }
    if(spinstop != nullptr)
    {
        spinstop->setMinimum(min);
        spinstop->setMaximum(max);
        if(spinstop->value() > max)
            spinstop->setValue(max);
        else if(spinstop->value() < min)
            spinstop->setValue(min);
    }
}

void MainWindow::LoadFileList()
{
    std::fstream flist;
    flist.open("filelist_config.txt", std::ios::in);
    if(!flist.is_open())
        return;

    bool configfile = false;
    bool tdacfile   = false;
    std::string text;
    std::getline(flist, text);
    while(!flist.eof())
    {
        if(text[0] == '#')
        {
            if(std::string(text).substr(2).compare("Configfiles") == 0)
            {
                configfile = true;
                tdacfile   = false;
            }
            else if(std::string(text).substr(2).compare("TDACfiles") == 0)
            {
                configfile = false;
                tdacfile   = true;
            }
        }
        else
        {
            if(configfile)
                ui->CB_SaveLoad_Filename->addItem(text.c_str());
            else if(tdacfile)
                ui->CB_SaveLoad_Filename_TDACs->addItem(text.c_str());
            else
                logit("not implemented");
        }

        std::getline(flist, text);
    }

    if(ui->CB_SaveLoad_Filename->count() > 0)
        ui->CB_SaveLoad_Filename->setCurrentIndex(0);
    if(ui->CB_SaveLoad_Filename_TDACs->count() > 0)
        ui->CB_SaveLoad_Filename_TDACs->setCurrentIndex(0);
}

void MainWindow::SaveFileList()
{
    std::fstream flist;
    flist.open("filelist_config.txt", std::ios::out);
    flist << "# Configfiles" << std::endl;
    for(int i = 0; i < ui->CB_SaveLoad_Filename->count(); ++i)
        flist << ui->CB_SaveLoad_Filename->itemText(i).toStdString() << std::endl;
    flist << "# TDACfiles" << std::endl;
    for(int i = 0; i < ui->CB_SaveLoad_Filename_TDACs->count(); ++i)
        flist << ui->CB_SaveLoad_Filename_TDACs->itemText(i).toStdString() << std::endl;
    //TODO: TDAC file list missing
    flist.close();

}

std::string MainWindow::FindFileName(std::string filenameprefix, std::string filenamesuffix)
{
    std::fstream f;
    int fileindex = 0;
    do{
        f.close();
        std::stringstream s("");
        s << filenameprefix << ++fileindex << filenamesuffix;
        f.open(s.str().c_str(), std::ios::in);
    }while(f.is_open());

    std::stringstream s("");
    s << filenameprefix << fileindex << filenamesuffix;

    return s.str();
}

std::string MainWindow::WriteToFile(std::string filenameprefix, std::string filenamesuffix, std::string data)
{
    std::fstream f;
    std::string filename = FindFileName(filenameprefix, filenamesuffix);
    f.open(filename.c_str(), std::ios::out | std::ios::app | std::ios::binary);

    f << data;
    f.flush();
    f.close();

    return filename;
}

void SetTrimThreshold(double thr, void* mw)
{
    static MainWindow* lastmw = nullptr;

    if(mw != nullptr)
        lastmw = static_cast<MainWindow*>(mw);

    std::stringstream s("");
    s << thr;

    if(lastmw != nullptr)
    {
        lastmw->ui->LE_Trim_Threshold->setText(s.str().c_str());
        lastmw->ProcessEvents();
    }
}

void MainWindow::on_B_Injection_StartStop_clicked()
{
    // Nexys Not Open
    if(!nexys->is_open()) {
        logit("Nexys Board not opened!");
        return;
    }
    if(!nexys->is_programmed()) {
        logit("Nexys Board not programmed!");
        return;
    }

    if(ui->B_Injection_StartStop->text().toStdString().find("Start Injections") != std::string::npos)
    {
        ConfigureInjections(false);
        if(StartInjections(true))
        {
            ui->B_Injection_StartStop->setText("Stop Injections");
            logit("Injections started");
        }
        else
            logit("Error: Injection configuration failed");
    }
    else  //stop injections
    {
        if(ui->B_Injection_StartStop->text().toStdString().find('!') != std::string::npos)
            ui->B_Injection_StartStop->setText("Start Injections (!)");
        else
            ui->B_Injection_StartStop->setText("Start Injections");

        if(StopInjections(true))
            logit("Injections stopped");
        else
            logit("Error: Injection stopping failed");
    }
}

void MainWindow::on_B_Injection_Restart_clicked()
{
    // Nexys Not Open
    if(!nexys->is_open()) {
        logit("Nexys Board not opened!");
        return;
    }
    if(!nexys->is_programmed()) {
        logit("Nexys Board not programmed!");
        return;
    }

    if(RestartInjections(true))
        logit("Injections restarted");
    else
        logit("Error: Injection Restart failed");
}

void MainWindow::on_B_Config_Injection_clearall_clicked()
{
    for(unsigned int i = 0; i < MainWindow::rows; ++i)
    {
        std::stringstream s("");
        s << "eninj_row_" << i;
        atlaspix_row.SetParameter(s.str(), 0);
    }
    for(unsigned int i = 0; i < MainWindow::columns; ++i)
    {
        std::stringstream s("");
        s << "en_inject_col_" << i;
        atlaspix_column.SetParameter(s.str(), 0);
    }

    UpdateFromConfig();
}

void MainWindow::on_B_Config_Injection_wholerow_clicked()
{
    for(unsigned int i = 0; i < MainWindow::columns; ++i)
    {
        std::stringstream s("");
        s << "en_inject_col_" << i;
        atlaspix_column.SetParameter(s.str(), 1);
    }
    //turn all other rows off:
    for(unsigned int i = 0; i < MainWindow::rows; ++i)
    {
        std::stringstream s("");
        s << "eninj_row_" << i;
        atlaspix_row.SetParameter(s.str(), 0);
    }
    std::stringstream s("");
   // s << "eninj_row_" << ui->SB_Config_Injection_Row->value();
    atlaspix_row.SetParameter(s.str(), 1);

   // ui->CB_Config_Injection->setChecked(true);

    UpdateFromConfig();
}

void MainWindow::on_B_Config_Injection_wholecol_clicked()
{
    //turn all other columns off:
    for(unsigned int i = 0; i < MainWindow::columns; ++i)
    {
        std::stringstream s("");
        s << "en_inject_col_" << i;
        atlaspix_column.SetParameter(s.str(), 0);
    }
    for(unsigned int i = 0; i < MainWindow::rows; ++i)
    {
        std::stringstream s("");
        s << "eninj_row_" << i;
        atlaspix_row.SetParameter(s.str(), 1);
    }
    std::stringstream s("");
    //s << "en_inject_col_" << ui->SB_Config_Injection_Row->value();
    //atlaspix_column.SetParameter(s.str(), 1);

   // ui->CB_Config_Injection->setChecked(true);

    UpdateFromConfig();
}

void MainWindow::SB_Config_Injection_valueChanged(int arg1)
{
    (void) arg1;

    std::stringstream sc("");
    //sc << "en_inject_col_" << ui->SB_Config_Injection_Col->value();

    std::stringstream sr("");
   // sr << "eninj_row_" << ui->SB_Config_Injection_Row->value();

    //if(atlaspix_column.GetParameter(sc.str()) != 0 && atlaspix_row.GetParameter(sr.str()) != 0)
    //    ui->CB_Config_Injection->setChecked(true);
    //else
    //    ui->CB_Config_Injection->setChecked(false);
}

void MainWindow::on_CB_Config_Injection_clicked(bool checked)
{
    std::stringstream sc("");
   // sc << "en_inject_col_" << ui->SB_Config_Injection_Col->value();

    std::stringstream sr("");
   // sr << "eninj_row_" << ui->SB_Config_Injection_Row->value();

    if(checked)
    {
        atlaspix_column.SetParameter(sc.str(), 1);
        atlaspix_row.SetParameter(sr.str(), 1);
    }
    //TODO: think of a better possibility for having several pixels activated at once:
    else
    {
        atlaspix_column.SetParameter(sc.str(), 0);
        atlaspix_row.SetParameter(sr.str(), 0);
    }

    UpdateFromConfig();
}

void MainWindow::on_SB_Config_AmpOut_HB_Col_valueChanged(int arg1)
{
    std::stringstream sa("");
    sa << "en_ampout_col_" << arg1;

    std::stringstream sh("");
    sh << "en_hitbus_col_" << arg1;

   // ui->CB_Config_AmpOut->setChecked(atlaspix_column.GetParameter(sa.str()) != 0);
   // ui->CB_Config_HB->setChecked(atlaspix_column.GetParameter(sh.str()) != 0);
}

void MainWindow::CB_Config_AmpOut_HB_stateChanged(bool checked)
{
    QObject* obj = sender();
    std::stringstream s("");
   //if(obj == ui->CB_Config_HB)
   //     s << "en_hitbus_col_" << ui->SB_Config_AmpOut_HB_Col->value();
   // else if(obj == ui->CB_Config_AmpOut)
   //     s << "en_ampout_col_" << ui->SB_Config_AmpOut_HB_Col->value();
   // else
   // {
   //     logit("Error: unknown sender in AmpOut/HB state changed");
   //     return;
   // }

    atlaspix_column.SetParameter(s.str(), (checked != 0)?1:0);

    //update GUI for AmpOut/Hitbus:
    for(unsigned int i = 0; i < dac_labels.size(); ++i)
    {
        if(dac_labels[i]->text().toStdString().compare(s.str()) == 0)
        {
            dac_checkboxes[i]->setChecked(checked != 0);
            break;
        }
    }
}

void MainWindow::on_B_Config_AmpOut_HB_Clear_clicked()
{
    for(int i = 0; i < MainWindow::columns; ++i)
    {
        std::stringstream sa("");
        sa << "en_ampout_col_" << i;

        std::stringstream sh("");
        sh << "en_hitbus_col_" << i;

        atlaspix_column.SetParameter(sa.str(), 0);
        atlaspix_column.SetParameter(sh.str(), 0);
    }

    UpdateFromConfig();
}

void MainWindow::on_SB_TDAC_Config_TDAC_valueChanged(int arg1)
{
   /* if(!tdacs.SetTDACValue(static_cast<unsigned int>(ui->SB_TDAC_Config_Col->value()),
                           static_cast<unsigned int>(ui->SB_TDAC_Config_Row->value()),
                           static_cast<unsigned int>(arg1)))
        ui->SB_TDAC_Config_TDAC->setValue(
                    tdacs.GetTDACValue(static_cast<unsigned int>(ui->SB_TDAC_Config_Col->value()),
                                     static_cast<unsigned int>(ui->SB_TDAC_Config_Row->value())));
                                     */
}

void MainWindow::on_B_TDAC_Config_SetAllToValue_clicked()
{
   // tdacs.SetAllToValue(static_cast<unsigned int>(ui->SB_TDAC_Config_TDAC->value()));
}

void MainWindow::SB_TDAC_Address_valueChanged(int arg1)
{
   /* (void) arg1;
    ui->SB_TDAC_Config_TDAC->setValue(
                tdacs.GetTDACValue(static_cast<unsigned int>(ui->SB_TDAC_Config_Col->value()),
                                   static_cast<unsigned int>(ui->SB_TDAC_Config_Row->value())));
                                   */
}

void MainWindow::on_B_TDAC_Config_WriteRow_clicked()
{
    /*int row = ui->SB_TDAC_Config_Row->value();

    if(!config.WriteRAMRow(row))
        logit("Writing RAM not successful");
    else
        logit("RAM written for row " + QString::number(row).toStdString());*/
}

void MainWindow::on_B_TDAC_Config_WriteMatrix_clicked()
{
    if(config.WriteRAMMatrix())
        logit("Writing Matrix successful");
    else
        logit("Error writing matrix");
}

void MainWindow::on_B_PinConfig_Update_clicked()
{
    if(!nexys->is_open())
        return;
/*
    if(config.ConfigurePins(true))
    {
        unsigned int pos = ui->B_PinConfig_Update->text().toStdString().find(" (!)");
        if(pos != static_cast<unsigned int>(std::string::npos))
            ui->B_PinConfig_Update->setText(ui->B_PinConfig_Update->text().toStdString().substr(0,pos).c_str());

        logit("FPGA pins updated");
    }*/
}

void MainWindow::on_CB_Configuration_Config_currentIndexChanged(int index)
{
    ui->GB_SPI->setVisible(index == 2);
    ui->GB_CMD->setVisible(index == 3);

    // turn on / off cmd decoder module on FPGA:
    if(index == 3)
        nexys->Write(50, 1);
    else
        nexys->Write(50, 0);

    config.SetConfigMeans(index);

//#define SPI_AUTOCONFIG

#ifdef SPI_AUTOCONFIG
    if(index == 0)
    {
        fm_configmode.SetFlag("useSPI", 0);
        fm_configmode.SetFlag("bypassCMD", 1);

        on_B_PinConfig_Update_clicked();
    }
    else if(index == 1)
    {
        fm_configmode.SetFlag("useSPI", 0);
        fm_configmode.SetFlag("bypassCMD", 1);

        on_B_PinConfig_Update_clicked();
    }
    else if(index == 2)
    {
        fm_configmode.SetFlag("useSPI", 1);
        fm_configmode.SetFlag("bypassCMD", 1);
    }
    else if(index == 3)
    {
        fm_configmode.SetFlag("useSPI", 0);
        fm_configmode.SetFlag("bypassCMD", 0);
    }
    else
        logit("Unsupported Option chosen. This should not happen");
#else
    logit("please configure the chip accordingly!");
#endif
}

void MainWindow::on_SB_SPI_ClockDiv_valueChanged(int arg1)
{
    if(!spiconfig.SetClockDivider(nexys, byte(arg1)) && nexys->is_open())
        logit("Error Updating SPI Clock Divider");
}

void MainWindow::on_CB_SPI_ReadbackEn_stateChanged(int arg1)
{
    (void) arg1;
    if(!spiconfig.SetReadBackEnable(nexys, ui->CB_SPI_ReadbackEn->isChecked()) && nexys->is_open())
        logit("Error setting SPI readback enable");
}

void MainWindow::on_CB_SPI_Enable_stateChanged(int arg1)
{
    (void) arg1;
    if(!spiconfig.SetSPIEnable(nexys, ui->CB_SPI_Enable->isChecked()) && nexys->is_open())
        logit("Error setting SPI enable");
}

void MainWindow::on_B_SPI_Reset_clicked()
{
    if(!spiconfig.ResetModule(nexys))
        logit("Error resetting SPI module");
}

void MainWindow::on_B_SPI_Reset_WriteFIFO_clicked()
{
    if(!spiconfig.ResetWriteFIFO(nexys))
        logit("Error resetting SPI write FIFO");
}

void MainWindow::on_B_SPI_Reset_ReadFIFO_clicked()
{
    if(!spiconfig.ResetReadFIFO(nexys))
        logit("Error resetting SPI read FIFO");
}


void MainWindow::on_B_SPI_Read_ReadFIFO_clicked()
{
    std::string answer = spiconfig.Read(nexys, 8);

    if(answer != "")
    {
        std::cout << "SPI readback: ";
        for(int i = 0; i < 8; ++i)
            std::cout << std::bitset<8>(static_cast<unsigned long long>(answer.c_str()[i])) << " ";
        std::cout << std::endl;
    }
}

void MainWindow::on_tabWidget_2_currentChanged(int index)
{
    if(ui->CB_Config_AutoChange->isChecked())
    {
        //turn all checkboxes off:
        if(index != 0)
            ui->CB_Config_DAC->setChecked(false);
        if(index != 1)
            ui->CB_Config_Config->setChecked(false);
        if(index != 2)
            ui->CB_Config_VDAC->setChecked(false);
        if(index < 3 || index == 4)
            ui->CB_Config_ColRow->setChecked(false);
        switch(index)
        {
        case(0):
            ui->CB_Config_DAC->setChecked(true);
            break;
        case(1):
            ui->CB_Config_Config->setChecked(true);
            break;
        case(2):
            ui->CB_Config_VDAC->setChecked(true);
            break;
        case(3):
        case(5):
        case(6):
        case(7):
            ui->CB_Config_ColRow->setChecked(true);
            break;
        default:
            break;
        }
    }
}

void MainWindow::SB_Trim_valueChanged(int arg1)
{
    QObject* obj = sender();

    std::stringstream s("");
    s << std::fixed << std::setprecision(3) << (arg1 / 255. * 1.8) << " V";

    if(obj == ui->SB_Trim_Start)
    {
        trim->SetIntervalStart(arg1);
        ui->L_Trim_Start->setText(s.str().c_str());
    }
    else if(obj == ui->SB_Trim_Step)
    {
        trim->SetIntervalStep(arg1);
        ui->L_Trim_Step->setText(s.str().c_str());
    }
    else if(obj == ui->SB_Trim_End)
    {
        trim->SetIntervalEnd(arg1);
        ui->L_Trim_End->setText(s.str().c_str());
    }
    else if(obj == ui->SB_ToTCal_Start)
        ui->L_ToTCal_Start->setText(s.str().c_str());
    else if(obj == ui->SB_ToTCal_Step)
        ui->L_ToTCal_Step->setText(s.str().c_str());
    else if(obj == ui->SB_ToTCal_End)
        ui->L_ToTCal_End->setText(s.str().c_str());
}

void MainWindow::on_SB_Trim_NumSignals_valueChanged(int arg1)
{
    trim->SetNumSignals(arg1);
}

void MainWindow::on_B_SCurve_clicked()
{
    static bool running = false;

    if(running)
    {
        running = false;
        return;
    }

    running = true;
    ui->B_SCurve->setText("Abort");
    QApplication::processEvents();

    std::map<Dataset, SCurve> data;

    if(ui->CB_SCurve_Del->isChecked())
        data = trim->MeasureSCurvesDelay(&running,true, -1);

    else if(!ui->CB_SCurve_Fast->isChecked())
        data = trim->MeasureSCurves(&running,true, -1);    

    else
        data = trim->MeasureFastSCurve(&running, ui->SB_ActivePixels->value(), ui->SB_Sleep->value());

    for(auto& it : data)
        it.second.SetMaximum(trim->GetNumSignals());
    double efficiency = trim->FitSCurves(data, false, 1.5);
    std::stringstream s("");
    s << " fitting efficiency: " << efficiency;
    logit(s.str());

    std::fstream f;
    std::string filename = FindFileName("SCurves_",".dat");
    f.open(filename.c_str(), std::ios::out | std::ios::app);

    for(auto& it : data)
    {
        std::stringstream s("");
        s << "Pixel (" << it.first.column << "|" << it.first.row << ")";
        f << it.second.GenerateString(s.str()) << std::endl;
    }

    f.flush();
    f.close();

    ui->B_SCurve->setText("Measure SCurve");
    running = false;
}

void MainWindow::on_B_TDAC_Config_SetRowToValue_clicked()
{
   /* for(unsigned int i = 0; i < columns; ++i)
        tdacs.SetTDACValue(i, static_cast<unsigned int>(ui->SB_TDAC_Config_Row->value()),
                           static_cast<unsigned int>(ui->SB_TDAC_Config_TDAC->value()));
                           */
}

void MainWindow::on_B_TDAC_Config_SetColToValue_clicked()
{
   /* for(unsigned int i = 0; i < rows; ++i)
        tdacs.SetTDACValue(static_cast<unsigned int>(ui->SB_TDAC_Config_Col->value()), i,
                           static_cast<unsigned int>(ui->SB_TDAC_Config_TDAC->value()));
                           */
}

void MainWindow::on_B_AllSCurves_clicked()
{
    static bool running = false;

    if(running)
    {
        running = false;
        return;
    }

    running = true;
    QString startcaption = ui->B_AllSCurves->text();
    ui->B_AllSCurves->setText("Abort");
    QApplication::processEvents();

    std::map<Dataset, SCurve> data = trim->MeasureAllSCurves(ui->CB_SCurve_Del->isChecked(), &running, false, true,
                                                             ui->SB_Trim_StartCol->value(), ui->SB_Trim_EndCol->value(),
                                                             ui->SB_Trim_StartRow->value(), ui->SB_Trim_EndRow->value());

    //data fitting:
    logit("Fitting SCurves...");
    //for(auto& it : data)
    //    it.second.SetMaximum(trim->GetNumSignals());
    double efficiency = trim->FitSCurves(data, false, 1.5);
    std::stringstream s("");
    s << " fitting efficiency: " << efficiency;
    logit(s.str());


    std::fstream f;
    std::string filename = FindFileName("AllSCurves_",".dat");
    f.open(filename.c_str(), std::ios::out | std::ios::app);

    for(auto& it : data)
    {
        std::stringstream s("");
        s << "Pixel (" << it.first.column << "|" << it.first.row << ") for TDAC=" << it.first.tdac;
        f << it.second.GenerateString(s.str()) << std::endl;
    }

    f.flush();
    f.close();

    Histogram hist_th;
    if(ui->CB_SCurve_Del->isChecked()) hist_th = trim->FillHistogram(data, 0, 63, 1);
    else hist_th = trim->FillHistogram(data, 0, 1.8, 0.025);
    hist_th.FindMinimum();
    trim->FitGaus(hist_th, false);

    std::string filenamehisto = FindFileName("AllSCurvesHisto_",".dat");
    hist_th.PrintToFile(filenamehisto);

    Histogram hist_n;
    if(ui->CB_SCurve_Del->isChecked()) hist_n = trim->FillHistogramWithNoise(data, 0, 5, 0.1);
    else hist_n = trim->FillHistogramWithNoise(data, 0, 0.18, 0.0025);
    hist_n.FindMinimum();
    trim->FitGaus(hist_n, false);

    std::string filenamehistonoise = FindFileName("AllSCurvesHistoNoise_",".dat");
    hist_n.PrintToFile(filenamehistonoise);



    logit("   Measuring SCurves done.");

    running = false;
    ui->B_AllSCurves->setText(startcaption);
}

void MainWindow::on_CB_Trig_PosEdge_clicked(bool checked)
{
    trigger_fm.SetFlag("posedge", (checked)?1:0);

    if(nexys->is_open())
        nexys->Write(byte(trigger_fm.GetFPGAAddress()), byte(trigger_fm.GetConfiguration()));
}

void MainWindow::on_CB_Trig_Source_currentIndexChanged(int index)
{
    trigger_fm.SetFlag("source", index);

    if(nexys->is_open())
        nexys->Write(byte(trigger_fm.GetFPGAAddress()), byte(trigger_fm.GetConfiguration()));
}

void MainWindow::on_CB_Trig_NoiseReject_currentIndexChanged(int index)
{
    trigger_fm.SetFlag("noise", index);

    if(nexys->is_open())
        nexys->Write(byte(trigger_fm.GetFPGAAddress()), byte(trigger_fm.GetConfiguration()));
}

void MainWindow::on_SB_Trig_PulseDelay_valueChanged(int arg1)
{
    if(!trigger_fm.SetFlag("delay", arg1))
        ui->SB_Trig_PulseDelay->setValue(trigger_fm.GetFlag("delay"));
    else if(nexys->is_open())
    {
        int delay = trigger_fm.GetFlag("delay");
        nexys->Write(byte(trigger_fm.GetFPGAAddress()+1), byte(delay >> 16), false);
        nexys->Write(byte(trigger_fm.GetFPGAAddress()+1), byte(delay >>  8), false);
        nexys->Write(byte(trigger_fm.GetFPGAAddress()+1), byte(delay),       true);
    }
}

void MainWindow::on_SB_Trig_PulseLength_valueChanged(int arg1)
{
    if(!trigger_fm.SetFlag("length", arg1))
        ui->SB_Trig_PulseDelay->setValue(trigger_fm.GetFlag("length"));
    else if(nexys->is_open())
    {
        int length = trigger_fm.GetFlag("length");
        nexys->Write(byte(trigger_fm.GetFPGAAddress()+2), byte(length >> 16), false);
        nexys->Write(byte(trigger_fm.GetFPGAAddress()+2), byte(length >>  8), false);
        nexys->Write(byte(trigger_fm.GetFPGAAddress()+2), byte(length),       true);
    }
}

void MainWindow::on_B_Trig_FTDITrigger_clicked()
{
    if(!nexys->is_open())
    {
        logit("No connection to Nexys");
        return;
    }

    trigger_fm.SetFlag("ftditrigger", 1);
    nexys->Write(byte(trigger_fm.GetFPGAAddress()), byte(trigger_fm.GetConfiguration()), false);
    trigger_fm.SetFlag("ftditrigger", 0);
    nexys->Write(byte(trigger_fm.GetFPGAAddress()), byte(trigger_fm.GetConfiguration()), true);

    logit("Manual Trigger sent");
}

void MainWindow::on_B_Trig_Update_clicked()
{
    if(!nexys->is_open())
        return;

    nexys->Write(byte(trigger_fm.GetFPGAAddress()), byte(trigger_fm.GetConfiguration()), false);
    on_SB_Trig_PulseDelay_valueChanged(-1);
    on_SB_Trig_PulseLength_valueChanged(-1);
}

void MainWindow::MeasureInjectionTiming(std::string archiveprefix, bool *running, bool updategui, int colmin, int colmax, int rowmin, int rowmax)
{
    if(running == nullptr || *running == false)
        return;

    if(nexys == nullptr || !nexys->is_open())
    {
        logit("no connection to nexys");
        return;
    }

    std::string filename = FindFileName(archiveprefix, ".dat");

    uint numsignals_backup  = injection.GetNumPulseSets();
    bool tsoverflow_backup = injection.GetTSOverflowSync();
    int numsignals         = ui->SB_OsciTiming_NumSignals->value();

    uint delay_backup       = injection.GetInitDelay();
    uint delay_start        = uint(ui->SB_OsciTiming_timeshift_start->value());
    uint delay_end          = uint(ui->SB_OsciTiming_timeshift_end->value());
    int  tstoobserve        = ui->SB_OsciTiming_TStoObserve->value();

    injection.SetNumPulseSets(uint(numsignals));
    injection.SetTSOverflowSync(true);
    injection.SetInitDelay(delay_start);

    bool selfblocked = false;
    if(!config.GetProgressBarBlocked())
    {
        config.SetProgressBarMaximum(int(delay_end - delay_start + 1));
        config.SetProgressBarValue(0);
        config.BlockProgressBar();
        config.ProcessEvents();
        selfblocked = true;
    }

    std::map<Dataset, SCurve> timingCurves;

    int noise = 0;

    //enlarge the read block size for measuring several pixels at once:
    int numpixels = 1;
    numpixels = (colmax - colmin + 1) * (rowmax - rowmin + 1);
    if(numpixels > 5)
        numpixels = 5;
    else if(numpixels <= 0)
        numpixels = 1;

    int skipped = 0;
    int stepstaken = 0;

    for(uint delay = delay_start; delay <= delay_end && *running; ++delay)
    {
        injection.SetInitDelay(delay);
        ConfigureInjections(false);

        StartInjections(true);

        Timing::Sleep(15 + numsignals / 100);

        std::vector<Dataset> hits = config.ReadoutAll(running, false, 0, 5, numpixels);

        int goodhits = 0;

        for(auto& it : hits)
        {
            if(*running == false)
                break;

            auto curveit = timingCurves.find(it);
            if(curveit != timingCurves.end())
            {
                if(curveit->first.ts == it.ts)
                {
                    curveit->second.AddHits(delay, 1);
                    ++goodhits;
                }
            }
            else if(it.column >= 0 && it.column < AP3columns && it.row >= 0 && it.row < AP3rows)
            {
                if(((rowmin >= 0 && it.row < rowmin) || (rowmax >= 0 && it.row > rowmax)) && updategui)
                {
                    //std::cout << " row out of range (" << rowmin << " - " << rowmax << "): " << it.row << std::endl;
                    ++noise;
                }
                else if(((colmin >= 0 && it.column < colmin) || (colmax >= 0 && it.column > colmax)) && updategui)
                {
                    //std::cout << " column out of range (" << colmin << " - " << colmax << "): " << it.column << std::endl;
                    ++noise;
                }
                else
                {
                    SCurve curve;
                    curve.SetMaximum(numsignals);
                    curve.AddPoint(delay, 0);
                    Dataset dat = it;
                    if(tstoobserve >= 0)
                    {
                        dat.ts = tstoobserve;
                        if(it.ts == tstoobserve)
                        {
                            curve.AddHits(delay, 1);
                            ++goodhits;
                        }
                    }
                    else
                        dat.ts = dat.ts + 2;
                    timingCurves.insert(std::make_pair(dat, curve));
                }

            }
        }

        //add a '0' to all existing timing curves without entry for the current delay value:
        for(auto& it : timingCurves)
        {
            if(it.second.GetSignalsToVolt(delay) < 0)
                it.second.AddPoint(delay, 0);
        }

        //progress indicator in cout:
        std::cout << ".";
        std::cout.flush();

        if(updategui)
        {
            switch(skipped)
            {
            case(0):
                config.SetProgressBarValue(config.GetProgressBarValue()+1);
                break;
            case(1):
                config.SetProgressBarValue(config.GetProgressBarValue()+2);
                break;
            case(2): //here the step has already been performed during skipping
            default:
                break;
            }
            config.ProcessEvents();
        }


        if(skipped == 2)
        {
            skipped = 0;
            ++delay; //skip measurement that has already been performed
        }
        else if(goodhits == 0)
        {
            skipped = 1;
            ++delay;
        }
        else if(skipped == 1) // && goodhits > 0) //already from last else if()
        {
            skipped = 2;
            delay -= 2; //due to loop adding again '1'
        }
        ++stepstaken;
    }

    std::cout << std::endl;
    std::cout << "rejected " << noise << " noise hits." << std::endl;
    std::cout << "     measured " << stepstaken << " steps instead of " << (delay_end - delay_start + 1) << std::endl;

    //fill missing points (0) at small delays:
    for(auto& it : timingCurves)
    {
        for(unsigned int delay = delay_start; delay <= delay_end; ++delay)
        {
            if(it.second.GetSignalsToVolt(delay) < 0)
                it.second.AddPoint(delay, 0);
            //else      //necessary due to skipping
            //    break;
        }

        it.second.Sort();
    }

    logit("Fitting Timing Curves ...");
    trim->FitWindows(timingCurves, false, delay_start + 2, 1.0, false);

    std::fstream f;
    f.open(filename.c_str(), std::ios::out | std::ios::app);

    if(f.is_open())
    {
        std::stringstream sdata("");
        for(auto& it : timingCurves)
        {
            std::stringstream stitle("");
            stitle << "Pixel (" << it.first.column << "|" << it.first.row << ") for TS " << it.first.ts;

            sdata << it.second.GenerateString(stitle.str(), false) << std::endl;
        }

        f << sdata.str();
    }
    f.close();

    injection.SetInitDelay(delay_backup);
    injection.SetNumPulseSets(uint(numsignals_backup));
    injection.SetTSOverflowSync(tsoverflow_backup);
    ConfigureInjections(false);
    StopInjections(true);

    if(selfblocked)
        config.ReleaseProgressBar();

    logit("Finished Timing Scan");
}

void MainWindow::on_B_OsciTiming_Pixel_clicked()
{
    static bool running = false;
    if(running)
    {
        running = false;
        return;
    }

    running = true;
    bool selfstarted = (sender() == ui->B_OsciTiming_Pixel);
    QString oldcaption = ui->B_OsciTiming_Pixel->text();
    if(selfstarted)
        ui->B_OsciTiming_Pixel->setText("Abort");

    else if(ui->RB_OsciTiming_timeshift->isChecked())
        MeasureInjectionTiming("Timing_", &running, true);

    if(selfstarted)
        ui->B_OsciTiming_Pixel->setText(oldcaption);

    running = false;
}

void MainWindow::on_B_OsciTiming_Matrix_clicked()
{
    static bool running = false;

    if(running || ui->B_OsciTiming_Matrix->text().toStdString().compare("Abort") == 0)
    {
        running = false;
        return;
    }
    running = true;
    QString oldcaption = ui->B_OsciTiming_Matrix->text();
    ui->B_OsciTiming_Matrix->setText("Abort");

    bool tsoverflow_backup = config.GetInjectionConfig()->GetTSOverflowSync();
    bool sync_backup = config.GetInjectionConfig()->GetSynced();
    config.GetInjectionConfig()->SetTSOverflowSync(true);

    int startcol = ui->SB_OsciTiming_StartCol->value();
    int endcol   = ui->SB_OsciTiming_EndCol->value();
    int colstep  = ui->SB_OsciTiming_ColStep->value();
    int numcolsatonce = ui->SB_OsciTiming_ColMultiple->value();

    int startrow = ui->SB_OsciTiming_StartRow->value();
    int endrow   = ui->SB_OsciTiming_EndRow->value();
    int rowstep  = ui->SB_OsciTiming_RowStep->value();
    int numrowsatonce = ui->SB_OsciTiming_RowMultiple->value();
    int numsteps = ((endcol - startcol) / colstep + 1) * ((endrow - startrow) / rowstep + 1);

    int numsignals = ui->SB_OsciTiming_NumSignals->value();

    if(ui->RB_OsciTiming_osci->isChecked())
        ui->progressBar->setMaximum(numsteps * (numsignals + 1));
    else if(ui->RB_OsciTiming_timeshift->isChecked())
    {
        int start = ui->SB_OsciTiming_timeshift_start->value();
        int end   = ui->SB_OsciTiming_timeshift_end->value();
        ui->progressBar->setMaximum(numsteps * (end - start + 2));
    }
    ui->progressBar->setValue(0);
    config.BlockProgressBar();
    QApplication::processEvents();

    bool autoupdate = ui->CB_Autoupdate->isChecked();
    ui->CB_Autoupdate->setChecked(false);

    //backup the settings
    config.GetInjectionConfig()->SetTSOverflowSync(tsoverflow_backup);
    config.GetInjectionConfig()->SetSynced(sync_backup);

    logit("   Finished Timing Matrix Scan");

    config.ReleaseProgressBar();
    ui->CB_Autoupdate->setChecked(autoupdate);
    ui->B_OsciTiming_Matrix->setText(oldcaption);
    running = false;
}


void MainWindow::on_RB_OsciTiming_osci_toggled(bool checked)
{
    ui->SB_OsciTiming_timeshift_start->setVisible(!checked);
    ui->SB_OsciTiming_timeshift_end->setVisible(!checked);
    ui->SB_OsciTiming_TStoObserve->setVisible(!checked);
    ui->SB_OsciTiming_Timeout->setEnabled(checked);
}

void MainWindow::on_B_Trig_ResetID_clicked()
{
    if(nexys == nullptr || !nexys->is_open())
        return;

    nexys->Write(43, 1, false, 8);
    nexys->Write(43, 0, true,  1);

    logit("Trigger ID reset");

}

void MainWindow::on_B_Trim_Test_clicked()
{
    for(int i = 7; i >= 0; --i)
    {
        std::stringstream s("");
        s << "Start Measurement for TDAC " << i << " ...";
        logit(s.str());

        config.GetTDACConfig()->SetAllToValue(uint(i));
        config.GetTDACConfig()->SetTDACValue(10, 19, 8);
        config.WriteRAMMatrix();

        on_B_AllSCurves_clicked();
    }
}

void MainWindow::on_B_Thr_Scan_clicked()
{
    static bool running = false;

    if(running)
    {
        running = false;
        on_B_AllSCurves_clicked();
        return;
    }

    running = true;

    ui->B_Thr_Scan->setText("Abort");
    QApplication::processEvents();

    for(int i = 145; i <= 159 && running; ++i)
    {
        config.GetATLASPixConfig(Configuration::vdac)->SetParameter("Th", uint(i));
        config.SendUpdate(Configuration::vdac,false);

        on_B_AllSCurves_clicked();

        std::time_t end_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::string timenow = std::ctime(&end_time);

        std::fstream f;
        f.open("Thr_Scan.log", std::ios::out | std::ios::app);

        if(f.is_open())
        {
            f << "Starting SCurves at Thr=" << i << " at " << timenow << std::endl;
            f.flush();
        }

        f.close();
    }

    ui->B_Thr_Scan->setText("Thr Scan");
    running = false;
}

void MainWindow::on_B_Trim_Trim_clicked()
{
    if(nexys == nullptr || !nexys->is_open())
        return;

    static bool running = false;
    static bool stopping = false;

    if(running)
    {
        logit("Aborting Trimming...");
        running = false;
        stopping = true;
        return;
    }
    else if(stopping)
        return;

    running = true;

    QString caption = ui->B_Trim_Trim->text();
    ui->B_Trim_Trim->setText("Abort");

    //make this instance available to show the target threshold:
    QString oldthr = ui->LE_Trim_Threshold->text();
    SetTrimThreshold(0, static_cast<void*>(this)); //passes the pointer to the function
    ui->LE_Trim_Threshold->setText(oldthr); //recovers the previous content

    trim->SetTriggerIDUsage(ui->CB_Trim_UseTrigID->isChecked());
    trim->SetMaxTime(ui->SB_Trim_MaxTime->value());

    double threshold;
    std::stringstream s("");
    s << ui->LE_Trim_Threshold->text().toStdString();
    if(s >> threshold)
    {
        trim->TrimChip(ui->CB_SCurve_Del->isChecked(), &running, &threshold, ui->CB_Trim_SkipVNDAC->isChecked(), ui->SB_Trim_StartCol->value(), ui->SB_Trim_EndCol->value(),
                       ui->SB_Trim_StartRow->value(), ui->SB_Trim_EndRow->value(), true);
    }
    else
        trim->TrimChip(ui->CB_SCurve_Del->isChecked(), &running, nullptr, ui->CB_Trim_SkipVNDAC->isChecked(), ui->SB_Trim_StartCol->value(), ui->SB_Trim_EndCol->value(),
                       ui->SB_Trim_StartRow->value(), ui->SB_Trim_EndRow->value(), true, SetTrimThreshold);


    running = false;
    stopping = false;
    ui->B_Trim_Trim->setText(caption);
}

bool MainWindow::ChangeConfig(std::string command)
{
    int configline = 0;
    bool pixelinject = false;
    bool tdac = false;
    bool configpins = false;
    bool injection = false;
    bool update = false;
    if(command.substr(0,3).compare("dac") == 0)
    {
        configline = Configuration::dac;
        command = command.substr(4);
    }
    else if(command.substr(0,6).compare("config") == 0)
    {
        configline = Configuration::config;
        command = command.substr(7);
    }
    else if(command.substr(0,4).compare("vdac") == 0)
    {
        configline = Configuration::vdac;
        command = command.substr(5);
    }
    else if(command.substr(0,6).compare("column") == 0)
    {
        configline = Configuration::column;
        command = command.substr(7);
    }
    else if(command.substr(0,3).compare("row") == 0)
    {
        configline = Configuration::row;
        command = command.substr(4);
    }
    else if(command.substr(0,6).compare("pixinj") == 0)
    {
        pixelinject = true;
        command = command.substr(7);
    }
    //hitbus not implemented
    else if(command.substr(0,4).compare("tdac") == 0)
    {
        tdac = true;
        command = command.substr(5);
    }
    else if(command.substr(0,10).compare("configpins") == 0)
    {
        configpins = true;
        command = command.substr(11);
    }
    else if(command.substr(0,9).compare("injconfig") == 0)
    {
        injection = true;
        command = command.substr(10);
    }
    else if(command.substr(0,4).compare("load") == 0)
    {
        if(command.substr(5,6).compare("config") == 0)
        {
            ui->CB_SaveLoad_Filename->setCurrentText(command.substr(12).c_str());
            ui->CB_SaveLoad_Filename_TDACs->setCurrentText("");
            on_B_Config_Load_clicked();
            return true;
        }
        else if(command.substr(5,4).compare("tdac") == 0)
        {
            ui->CB_SaveLoad_Filename->setCurrentText("");
            ui->CB_SaveLoad_Filename_TDACs->setCurrentText(command.substr(10).c_str());
            on_B_Config_Load_clicked();
            return true;
        }
        else
            return false;
    }
    else if(command.substr(0,4).compare("save") == 0)
    {
        if(command.substr(5,6).compare("config") == 0)
        {
            ui->CB_SaveLoad_Filename->setCurrentText(command.substr(12).c_str());
            ui->CB_SaveLoad_Filename_TDACs->setCurrentText("");
            on_B_Config_Save_clicked();
            return true;
        }
        else if(command.substr(5,6).compare("tdac") == 0)
        {
            ui->CB_SaveLoad_Filename->setCurrentText("");
            ui->CB_SaveLoad_Filename_TDACs->setCurrentText(command.substr(10).c_str());
            on_B_Config_Save_clicked();
            return true;
        }
        else
            return false;
    }
    else if(command.substr(0,4).compare("send") == 0)
    {
        update = true;
        command = command.substr(5);
    }

    if(configline != 0)
    {
        std::stringstream s("");
        s << command;
        std::string reg;
        int value;
        s >> reg >> value;
        ASIC_Config2* apconfig = config.GetATLASPixConfig(configline);
        bool result = false;
        if(apconfig != nullptr)
            result = apconfig->SetParameter(reg, uint(value));
        config.SendUpdate(configline);
        return result;
    }
    else if(pixelinject)
    {
        if(command.compare("clearall") == 0)
            on_B_Config_Injection_clearall_clicked();
        else if(command.substr(0,6).compare("column") == 0)
        {
           /* std::stringstream s("");
            s << command.substr(7);
            int col;
            s >> col;
            ui->SB_Config_Injection_Col->setValue(col);
            QApplication::processEvents();
            on_B_Config_Injection_wholecol_clicked();
            config.SendUpdate(Configuration::colrow);*/
        }
        else if(command.substr(0,3).compare("row") == 0)
        {
           /* std::stringstream s("");
            s << command.substr(4);
            int row;
            s >> row;
            ui->SB_Config_Injection_Row->setValue(row);
            QApplication::processEvents();
            on_B_Config_Injection_wholerow_clicked();
            config.SendUpdate(Configuration::colrow);*/
        }
        else
        {
            /*std::stringstream s("");
            s << command;
            int x,y;
            std::string onoff;
            s >> x >> y >> onoff;
            ui->SB_Config_Injection_Col->setValue(x);
            ui->SB_Config_Injection_Row->setValue(y);
            QApplication::processEvents();
            on_CB_Config_Injection_clicked((onoff.compare("on") == 0));
            QApplication::processEvents();
            config.SendUpdate(Configuration::colrow);*/
        }

        return true;
    }
    else if(tdac)
    {
        if(command.substr(0,3).compare("all") == 0)
        {
            std::stringstream s("");
            s << command.substr(4);
            int value;
            s >> value;
            bool result = !s.fail();
            if(result)
            {
                result = config.GetTDACConfig()->SetAllToValue(uint(value));
                config.WriteRAMMatrix();
            }
            return result;
        }
        else if(command.substr(0,3).compare("row") == 0)
        {
            std::stringstream s("");
            s << command.substr(4);
            int row, value;
            s >> row >> value;
            if(!s.fail())
            {
                /*ui->SB_TDAC_Config_Row->setValue(row);
                ui->SB_TDAC_Config_TDAC->setValue(value);
                QApplication::processEvents();
                on_B_TDAC_Config_SetRowToValue_clicked();
                config.WriteRAMRow(row);*/
                return true;
            }
            else
                return false;
        }
        else if(command.substr(0,6).compare("column") == 0)
        {
            std::stringstream s("");
            s << command.substr(7);
            int col, value;
            s >> col >> value;
            if(!s.fail())
            {
                /*ui->SB_TDAC_Config_Col->setValue(col);
                ui->SB_TDAC_Config_TDAC->setValue(value);
                QApplication::processEvents();
                on_B_TDAC_Config_SetColToValue_clicked();*/
                config.WriteRAMMatrix();
                return true;
            }
            else
                return false;
        }
        else
        {
            std::stringstream s("");
            s << command;
            int col, row, value;
            s >> col >> row >> value;
            if(!s.fail())
            {
                config.GetTDACConfig()->SetTDACValue(uint(col), uint(row), uint(value));
                config.WriteRAMRow(row);
                return true;
            }
            else
                return false;
        }
    }
    else if(configpins)
    {
        int flag = 0;
        if(command.substr(0,6).compare("resets") == 0)
        {
            flag = Configuration::pinconfig::resets;
            command = command.substr(7);
        }
        else if(command.substr(0,4).compare("work") == 0)
        {
            flag = Configuration::pinconfig::modework;
            command = command.substr(5);
        }
        else if(command.substr(0,6).compare("config") == 0)
        {
            flag = Configuration::pinconfig::modeconfig;
            command = command.substr(7);
        }

        std::stringstream s("");
        s << command;
        std::string name;
        int value;
        s >> name >> value;

        if(!s.fail() && flag != 0)
        {
            FlagManager* fm = config.GetPinConfig(flag);
            if(fm != nullptr)
            {
                fm->SetFlag(name, value);
                config.ConfigurePins();
                return true;
            }
            return false;
        }
        else
            return false;
    }
    else if(injection)
    {
        bool result = true;
        if(command.substr(0,7).compare("numsets") == 0)
        {
            std::stringstream s("");
            s << command.substr(8);
            int value;
            s >> value;
            if(!s.fail())
                config.GetInjectionConfig()->SetNumPulseSets(uint(value));
            else
                result = false;
        }
        else if(command.substr(0,9).compare("numperset") == 0)
        {
            std::stringstream s("");
            s << command.substr(10);
            int value;
            s >> value;
            if(!s.fail())
                config.GetInjectionConfig()->SetNumPulsesInaSet(uint(value));
            else
                result = false;
        }
        else if(command.substr(0,9).compare("initdelay") == 0)
        {
            std::stringstream s("");
            s << command.substr(10);
            int value;
            s >> value;
            if(!s.fail())
                config.GetInjectionConfig()->SetInitDelay(uint(value));
            else
                result = false;
        }
        else if(command.substr(0,6).compare("period") == 0)
        {
            std::stringstream s("");
            s << command.substr(7);
            int value;
            s >> value;
            if(!s.fail())
                config.GetInjectionConfig()->SetPeriod(uint(value));
            else
                result = false;
        }
        else if(command.substr(0,8).compare("clockdiv") == 0)
        {
            std::stringstream s("");
            s << command.substr(9);
            int value;
            s >> value;
            if(!s.fail())
                config.GetInjectionConfig()->SetClockDiv(uint(value));
            else
                result = false;
        }
        else if(command.substr(0,4).compare("mode") == 0)
        {
            if(command.substr(5).find("async") != std::string::npos)
            {
                config.GetInjectionConfig()->SetSynced(false);
                config.GetInjectionConfig()->SetTSOverflowSync(false);
            }
            else if(command.substr(5).find("tssync") != std::string::npos)
            {
                config.GetInjectionConfig()->SetTSOverflowSync(true);
                config.GetInjectionConfig()->SetSynced(true);
            }
            else if(command.substr(5).find("sync") != std::string::npos)
            {
                config.GetInjectionConfig()->SetTSOverflowSync(false);
                config.GetInjectionConfig()->SetSynced(true);
            }
            else
                result = false;
        }
        else if(command.substr(0,5).compare("start") == 0)
        {
            config.ConfigureInjections(false);
            config.StartInjections(true);
        }
        else if(command.substr(0,7).compare("restart") == 0)
        {
            config.RestartInjections(true);
        }
        else if(command.substr(0,4).compare("stop") == 0)
        {
            config.StopInjections(true);
        }
        else
            result = false;

        return result;
    }
    else if(update)
    {
        std::cout << "peep... \"" << command << "\"" << std::endl;
        if(command.substr(0,3).compare("dac") == 0)
            return config.SendUpdate(Configuration::dac, true);
        else if(command.substr(0,5).compare("config") == 0)
            return config.SendUpdate(Configuration::config, true);
        else if(command.substr(0,4).compare("vdac") == 0)
            return config.SendUpdate(Configuration::vdac, true);
        else if(command.substr(0,6).compare("column") == 0)
            return config.SendUpdate(Configuration::column, true);
        else if(command.substr(0,3).compare("row") == 0)
            return config.SendUpdate(Configuration::row, true);
        else if(command.substr(0,6).compare("colrow") == 0)
            return config.SendUpdate(Configuration::colrow, true);
        else if(command.substr(0,5).compare("flush") == 0)
            return config.GetNexysIO()->Flush();
        else
            return false;
    }
    else
        return false;
}

bool MainWindow::SCurveCommand(std::string command)
{
    bool result = true;
    if(command.substr(0,10).compare("numsignals") == 0)
    {
        std::stringstream s("");
        s << command.substr(11);
        int value;
        s >> value;
        if(!s.fail())
        {
            ui->SB_Trim_NumSignals->setValue(value);
            QApplication::processEvents();
        }
        else
            result = false;
    }
    else if(command.substr(0,8).compare("interval") == 0)
    {
        std::stringstream s("");
        s << command.substr(9);
        int start, step, end;
        s >> start >> step >> end;
        if(!s.fail())
        {
            ui->SB_Trim_Start->setValue(start);
            ui->SB_Trim_Step->setValue(step);
            ui->SB_Trim_End->setValue(end);
            QApplication::processEvents();
        }
        else
            result = false;
    }
    else if(command.substr(0,7).compare("measure") == 0)
        on_B_SCurve_clicked();
    else if(command.substr(0,10).compare("measureall") == 0)
    {
        std::stringstream s("");
        s << command.substr(11);
        int startcol, endcol, startrow, endrow;
        s >> startcol >> endcol >> startrow >> endrow;
        if(!s.fail())
        {
            ui->SB_Trim_StartCol->setValue(startcol);
            ui->SB_Trim_EndCol->setValue(endcol);
            ui->SB_Trim_StartRow->setValue(startrow);
            ui->SB_Trim_EndRow->setValue(endrow);
            QApplication::processEvents();
            on_B_AllSCurves_clicked();
        }
        else
            result = false;
    }
    else if(command.substr(0,8).compare("trimchip") == 0)
    {
        std::stringstream s("");
        s << command.substr(9);
        int startcol, endcol, startrow, endrow;
        std::string skipvndac = "";
        s >> skipvndac >> startcol >> endcol >> startrow >> endrow;
        if(!s.fail())
        {
            ui->CB_Trim_SkipVNDAC->setChecked(skipvndac.find("skip") != std::string::npos);
            ui->SB_Trim_StartCol->setValue(startcol);
            ui->SB_Trim_EndCol->setValue(endcol);
            ui->SB_Trim_StartRow->setValue(startrow);
            ui->SB_Trim_EndRow->setValue(endrow);
            QApplication::processEvents();
            on_B_Trim_Trim_clicked();
        }
        else
            result = false;
    }
    else
        result = false;

    return result;
}

bool MainWindow::TimingCommand(std::string command)
{
    bool result = true;
    if(command.substr(0,10).compare("numsignals") == 0)
    {
        std::stringstream s("");
        s << command.substr(11);
        int value;
        s >> value;
        if(!s.fail())
        {
            ui->SB_OsciTiming_NumSignals->setValue(value);
            QApplication::processEvents();
        }
        else
            result = false;
    }
    else if(command.substr(0,8).compare("interval") == 0)
    {
        std::stringstream s("");
        s << command.substr(9);
        int start, end;
        s >> start >> end;
        if(!s.fail())
        {
            ui->SB_OsciTiming_timeshift_start->setValue(start);
            ui->SB_OsciTiming_timeshift_end->setValue(end);
            QApplication::processEvents();
        }
        else
            result = false;
    }
    else if(command.substr(0,6).compare("target") == 0)
    {
        std::stringstream s("");
        s << command.substr(7);
        int value;
        s >> value;
        if(!s.fail())
        {
            ui->SB_OsciTiming_TStoObserve->setValue(value);
            QApplication::processEvents();
        }
        else
            result = false;
    }
    else if(command.substr(0,7).compare("measure") == 0)
    {
        std::stringstream s("");
        s << command.substr(8);
        int startcol, stepcol, endcol, multiplecol;
        int startrow, steprow, endrow, multiplerow;
        s >> startcol >> stepcol >> endcol >> multiplecol
          >> startrow >> steprow >> endrow >> multiplerow;

        if(!s.fail())
        {
            ui->SB_OsciTiming_StartCol->setValue(startcol);
            ui->SB_OsciTiming_ColStep->setValue(stepcol);
            ui->SB_OsciTiming_EndCol->setValue(endcol);
            ui->SB_OsciTiming_ColMultiple->setValue(multiplecol);

            ui->SB_OsciTiming_StartRow->setValue(startrow);
            ui->SB_OsciTiming_RowStep->setValue(steprow);
            ui->SB_OsciTiming_EndRow->setValue(endrow);
            ui->SB_OsciTiming_RowMultiple->setValue(multiplerow);

            QApplication::processEvents();

            on_B_OsciTiming_Matrix_clicked();
        }
        else
            result = false;
    }
    else
        result = false;

    return result;
}

void MainWindow::on_B_MeasureList_clicked()
{
    static bool running = false;

    if(running)
    {
        running = false;
        return;
    }

    running = true;
    QString caption = ui->B_MeasureList->text();
    ui->B_MeasureList->setText("Abort");
    QApplication::processEvents();

    std::string logfile = FindFileName("MeasureList_", ".log");

    Timing::TimePoint start = Timing::GetTimeNow();

    Configuration::WriteToFile(logfile, "[" + Timing::TimePointToTimeText(start) + "] Started process\n");

    while(running)
    {
        //load the text/list from the GUI:
        QString measuretext = ui->TE_MeasureList->toPlainText();
        QStringList qmeasurelist = measuretext.split(QRegExp("[\n]"), QString::SkipEmptyParts);
        std::string measuretask = "";
        int taskindex = -1;

        //sort by executed tasks ('[DONE]' tag):
        for(int i = 0; i < qmeasurelist.size(); ++i)
        {
            std::string line = qmeasurelist[i].toStdString();
            //std::cout << "Item " << i << ": " << line << std::endl;
            if(line.find("[DONE]") == std::string::npos)
            {
                measuretask = line;
                taskindex = i;
                break;
            }
        }
        if(measuretask == "")
            break;

        Configuration::WriteToFile(logfile, "[" + Timing::TimePointToTimeText(Timing::GetTimeNow()) + "] Starting \"" + measuretask + "\"\n");
        std::cout << "initiating \"" << measuretask << "\" ..." << std::endl;

        //perform task:
        bool result = true;
        //change configuration:
        if(measuretask.substr(0,6).compare("config") == 0)
            result = ChangeConfig(measuretask.substr(7));
        //delay:
        else if(measuretask.substr(0,5).compare("sleep") == 0)
        {
            std::stringstream s("");
            s << measuretask.substr(6);
            int value;
            s >> value;
            if(!s.fail())
                Timing::Sleep(value);
            else
                result = false;
        }
        else if(measuretask.substr(0,4).compare("exit") == 0)
        {
            result = true;
            running = false;
        }
        //SCurves and Trimming:
        else if(measuretask.substr(0,6).compare("scurve") == 0)
            result = SCurveCommand(measuretask.substr(7));
        else if(measuretask.substr(0,6).compare("timing") == 0)
            result = TimingCommand(measuretask.substr(7));
        else if(measuretask.substr(0,4).compare("echo") == 0)
        {
            config.logit(measuretask.substr(5));
            Configuration::WriteToFile(logfile, "Echo: \"" + measuretask.substr(5) + "\"\n");
        }
        else if(measuretask.substr(0,8).compare("regwrite") == 0)
        {
            std::stringstream s("");
            int reg, val;
            s << measuretask.substr(9);
            s >> reg >> val;
            if(!s.fail())
                result = nexys->Write(byte(reg), byte(val));
        }
        else
            result = false;

        if(!result)
        {
            config.logit("task \"" + measuretask + "\" failed!");
            Configuration::WriteToFile(logfile, " ... failed at " + Timing::TimePointToTimeText(Timing::GetTimeNow()) + "\n");
        }

        //try to find the first occurrence of the executed task in the QTextEdit (in case
        //   something changed during execution):
        QTextCursor cursor = ui->TE_MeasureList->textCursor();
        cursor.movePosition(QTextCursor::Start);

        //try all lines:
        int done = 0;
        while(done < 2)
        {
            cursor.select(QTextCursor::LineUnderCursor);
            QString line = cursor.selectedText();
            //std::cout << "testing: \"" << line.toStdString() << "\" ..." << std::endl;
            if(line.toStdString().compare(measuretask) == 0)
            {
                cursor.removeSelectedText();
                if(result)
                    cursor.insertText(QString("[DONE] ") + qmeasurelist[taskindex]);
                else
                    cursor.insertText(QString("[DONE] [FAILED] ") + qmeasurelist[taskindex]);
                break;
            }
            else
                cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor);

            if(cursor.atEnd())
                ++done;
        }
        //std::cout << "performed 1 task" << std::endl;
        QApplication::processEvents();
    }

    Timing::TimePoint ende = Timing::GetTimeNow();

    std::stringstream slog("");

    if(running)
        slog << "[" << Timing::TimePointToTimeText(ende) << "] Finished Task List" << std::endl;
    else
        slog << "[" << Timing::TimePointToTimeText(ende) << "] Process aborted" << std::endl;

    slog << "\t\tTotal duration: " << Timing::TimesToInterval(start, ende) << std::endl;

    UpdateFromConfig();

    Configuration::WriteToFile(logfile, slog.str());

    config.logit(slog.str());

    running = false;
    ui->B_MeasureList->setText(caption);
}

void MainWindow::on_B_MeasureList_Reset_clicked()
{
    std::string data = ui->TE_MeasureList->toPlainText().toStdString();

    unsigned int pos = data.find("[DONE] ");

    while(pos != uint(std::string::npos))
    {
        //remove the occurrence of "[DONE] ":
        data = data.substr(0,pos) + data.substr(pos + 7);
        //find the next occurrence:
        pos = data.find("[DONE] ", pos + 1);
    }

    //repeat for failed tasks:
    pos = data.find("[FAILED] ");
    while(pos != uint(std::string::npos))
    {
        //remove the occurrence of "[FAILED] ":
        data = data.substr(0, pos) + data.substr(pos + 9);
        //find the next occurrence:
        pos = data.find("[FAILED] ", pos + 1);
    }

    ui->TE_MeasureList->setText(QString(data.c_str()));
}

void MainWindow::on_CB_UDP_Debug_clicked(bool checked)
{
    if(nexys != nullptr && nexys->is_open())
    {
        udpro.SetDebugMode(!checked);
        //turn on UDP data sender:
        if(!checked)
            logit("Started UDP data sender");
        //turn off UDP data sender (->answerbot):
        else
            logit("Stopped UDP data sender (answerbot active)");

        nexys->Write(byte(udpro.GetConfigAddress()), udpro.GetConfigData(), true);
    }
    else
        ui->CB_UDP_Debug->setChecked(false);
}

void MainWindow::on_CB_UDP_Reset_clicked(bool checked)
{
    if(nexys != nullptr && nexys->is_open())
    {
        udpro.SetUDPReset(checked);

        if(checked)
            logit("Reset UDP Module.");
        else
            logit("UDP Module activated");

        nexys->Write(byte(udpro.GetConfigAddress()), udpro.GetConfigData(), true);
    }
    else
        ui->CB_UDP_Reset->setChecked(false);
}

void MainWindow::on_CB_UDP_Reset_Sender_clicked(bool checked)
{
    if(nexys != nullptr && nexys->is_open())
    {
        udpro.SetSenderReset(checked);

        if(checked)
            logit("Reset Fast Readout Data Sender");
        else
            logit("Fast Readout Data Sender activated");

        nexys->Write(byte(udpro.GetConfigAddress()), udpro.GetConfigData(), true);
    }
    else
        ui->CB_UDP_Reset_Sender->setChecked(false);
}

void MainWindow::on_CB_UDP_Reset_FIFO_clicked(bool checked)
{
    if(nexys != nullptr && nexys->is_open())
    {
        udpro.SetFIFOReset(checked);

        if(checked)
            logit("UDP Readout FIFO reset");
        else
            logit("UDP Readout FIFO active");

        nexys->Write(byte(udpro.GetConfigAddress()), udpro.GetConfigData(), true);
    }
    else
        ui->CB_UDP_Reset_FIFO->setChecked(false);
}

void MainWindow::on_SB_UDP_InterpackageDelay_valueChanged(int arg1)
{
    if(nexys != nullptr && nexys->is_open())
    {
        if(udpro.SetInterPackageDelay(arg1))
        {
            logit("Updated UDP Interpackage Delay");
            nexys->Write(byte(udpro.GetInterPackageDelayAddress()), udpro.GetInterPackageDelayData(), true);
        }
        else
            ui->SB_UDP_InterpackageDelay->setValue(udpro.GetInterPackageDelay());
    }
    else
        ui->SB_UDP_InterpackageDelay->setValue(udpro.GetInterPackageDelay());
}


void MainWindow::on_SB_UDP_Timeout_valueChanged(int arg1)
{
    if(nexys != nullptr && nexys->is_open())
    {
        if(udpro.SetTimeout(arg1))
        {
            logit("UDP package Timeout updated");
            nexys->Write(byte(udpro.GetTimeoutAddress()), udpro.GetTimeoutData(), true);
        }
        else
            ui->SB_UDP_Timeout->setValue(udpro.GetTimeout());
    }
    else
        ui->SB_UDP_Timeout->setValue(udpro.GetTimeout());
}


void MainWindow::on_LE_UDP_IP_textChanged(const QString &arg1)
{
    if(nexys != nullptr && nexys->is_open())
    {
        std::string caption = ui->B_UDP_Update->text().toStdString();
        if(arg1.toStdString().compare(udpro.GetIP()) != 0)
        {
            if(caption.find('!') != std::string::npos)
                ui->B_UDP_Update->setText((caption + " (!)").c_str());
        }
    }
    else
        ui->LE_UDP_IP->setText(udpro.GetIP().c_str());

}

void MainWindow::on_SB_UDP_Port_valueChanged(int arg1)
{
    if(nexys != nullptr && nexys->is_open())
    {
        if(udpro.SetFPGAPort(arg1))
        {
            logit("UDP Port on FPGA updated");
            nexys->Write(byte(udpro.GetPortAddress()), udpro.GetPortData(), true);
        }
        else
            ui->SB_UDP_Port->setValue(udpro.GetFPGAPort());
    }
    else
        ui->SB_UDP_Port->setValue(udpro.GetFPGAPort());
}

void MainWindow::on_LE_UDP_SubnetMask_textChanged(const QString &arg1)
{
    if(nexys != nullptr && nexys->is_open())
    {
        std::string caption = ui->B_UDP_Update->text().toStdString();
        if(arg1.toStdString().compare(udpro.GetSubnetMask()) != 0)
        {
            if(caption.find('!') != std::string::npos)
                ui->B_UDP_Update->setText((caption + " (!)").c_str());
        }
    }
    else
        ui->LE_UDP_SubnetMask->setText(udpro.GetSubnetMask().c_str());
}

void MainWindow::on_LE_UDP_MAC_textChanged(const QString &arg1)
{
    if(nexys != nullptr && nexys->is_open())
    {
        std::string caption = ui->B_UDP_Update->text().toStdString();
        if(arg1.toStdString().compare(udpro.GetMAC()) != 0)
        {
            if(caption.find('!') != std::string::npos)
                ui->B_UDP_Update->setText((caption + " (!)").c_str());
        }
    }
    else
        ui->LE_UDP_MAC->setText(udpro.GetMAC().c_str());
}

void MainWindow::on_B_UDP_Update_clicked()
{
    if(nexys != nullptr && nexys->is_open())
    {
        if(!udpro.SetIP(ui->LE_UDP_IP->text().toStdString()))
            logit("Error in IP Address, not sent");
        else
            nexys->Write(byte(udpro.GetIPAddress()), udpro.GetIPData(), true);

        if(!udpro.SetSubnetMask(ui->LE_UDP_SubnetMask->text().toStdString()))
            logit("Error in Subnet Mask, not sent");
        else
            nexys->Write(byte(udpro.GetSubnetMaskAddress()), udpro.GetSubnetMaskData(), true);

        if(!udpro.SetMAC(ui->LE_UDP_MAC->text().toStdString()))
            logit("Error in MAC Address, not sent");
        else
            nexys->Write(byte(udpro.GetMACAddress()), udpro.GetMACData(), true);

        nexys->Write(byte(udpro.GetPortAddress()), udpro.GetPortData(), true);
        nexys->Write(byte(udpro.GetTimeoutAddress()), udpro.GetTimeoutData(), true);
        nexys->Write(byte(udpro.GetInterPackageDelayAddress()), udpro.GetInterPackageDelayData(), true);
        nexys->Write(byte(udpro.GetConfigAddress()), udpro.GetConfigData(), true);

        logit("UDP configuration sent");

        std::string caption = ui->B_UDP_Update->text().toStdString();
        if(caption.find('!') != std::string::npos)
            ui->B_UDP_Update->setText(caption.substr(0, caption.length()-4).c_str());
    }
}

void MainWindow::on_B_Inject_CMD_clicked(bool checked)
{
    if(nexys == nullptr || !nexys->is_open() || config.GetConfigMeans() != Configuration::cmd)
        return;

    cmd.SetConfigBit("injection", checked);
    cmd.SetConfigBit("selectconfig", checked);
    std::vector<byte> command = cmd.GenerateSetBitByteCode(16, 1);

    config.SendCMDSplit(command);
    nexys->Flush();
}

void MainWindow::on_B_CMD_Send_clicked()
{
    if(nexys == nullptr || !nexys->is_open() || config.GetConfigMeans() != Configuration::cmd)
        return;

    std::stringstream s("");
    s << ui->LE_CMD_CMD->text().toStdString();
    int value = 0;
    std::vector<int> cmdcontent;
    while(s >> value)
        cmdcontent.push_back(value);

    std::vector<byte> cmdcode;
    std::cout << "Entered Command: ";
    for(auto& it : cmdcontent)
    {
        cmdcode.push_back(it);
        std::cout << it << " ";
    }

    std::cout << std::endl;

    config.SendCMDSplit(cmdcode);
    config.GetNexysIO()->Flush();
}

void MainWindow::on_B_ToTCal_Start_clicked()
{
    if(nexys == nullptr || !nexys->is_open())
    {
        logit("no Connection to Nexys to perform ToTCalibration");
        return;
    }

    static bool running = false;

    if(running)
    {
        running = false;
        return;
    }

    running = true;
    QString caption = ui->B_ToTCal_Start->text();
    ui->B_ToTCal_Start->setText("Abort");
    logit("Starting ToTCalibration Scan...");
    QApplication::processEvents();

    int startval   = ui->SB_ToTCal_Start->value();
    int stepval    = ui->SB_ToTCal_Step->value();
    int endval     = ui->SB_ToTCal_End->value();
    int numsignals = ui->SB_ToTCal_NumSignals->value();
    bool debug     = ui->CB_ToTCal_SaveHists->isChecked();

    Rect measurearea;
    measurearea.startcol = ui->SB_ToTCal_StartCol->value();
    measurearea.startrow = ui->SB_ToTCal_StartRow->value();
    measurearea.endcol   = ui->SB_ToTCal_EndCol->value();
    measurearea.endrow   = ui->SB_ToTCal_EndRow->value();

    ToTCalibration cali(config);

    cali.SetInterval_start(startval);
    cali.SetInterval_step(stepval);
    cali.SetInterval_end(endval);
    cali.SetNumSignals(numsignals);
    cali.SetTSdiv(config.GetATLASPixConfig(Configuration::config)->GetParameter("ckdivend")+1);
    cali.SetTS2div(config.GetATLASPixConfig(Configuration::config)->GetParameter("ckdivend2")+1);

    std::string filename = FindFileName("ToT_Calibration_", ".dat");
    std::string histfilename = (debug)?FindFileName("ToTCalibration_histograms_",".dat"):"";
    /*std::map<Dataset, PointCurve> result =*/ cali.MeasureToTArea(&running, 20, measurearea,
                                                               filename, histfilename);

    //for(auto& it : result)
    //    WriteToFile(filename, it.second.GenerateString(it.first.ToAddressString(), false) + "\n");

    if(running)
        logit(" finished ToTCalibration Scan");
    else
        logit(" ToTCalibration Scan aborted");
    running = false;
    ui->B_ToTCal_Start->setText(caption);
}

void MainWindow::on_B_CMD_Trig_Send_clicked()
{
    if(nexys == nullptr || !nexys->is_open() || config.GetConfigMeans() != Configuration::cmd)
        return;

    int triggercode = config.GetCMDConfig()->GetTriggerCode(ui->CB_CMD_Trig1->isChecked(),
                                                            ui->CB_CMD_Trig2->isChecked(),
                                                            ui->CB_CMD_Trig3->isChecked(),
                                                            ui->CB_CMD_Trig4->isChecked());
    static int triggertag = 0;
    if(triggertag < 31)
        ++triggertag;
    else
        triggertag = 0;

    std::vector<byte> data = config.GetCMDConfig()->GenerateTriggerCode(triggercode, triggertag);

    if(!config.SendCMDSplit(data))
        logit("Sending CMD Trigger command failed");
    else
        logit("Trigger Command sent");

    nexys->Flush();
}

void MainWindow::on_SB_Trim_MaxSimPixels_valueChanged(int arg1)
{
    trim->SetMaxSimultaneousPixels(arg1);
}


void MainWindow::on_CB_Trim_UseTrigID_clicked(bool checked)
{
    trim->SetTriggerIDUsage(checked);
}

void MainWindow::on_SB_Trim_MaxTime_valueChanged(int arg1)
{
    trim->SetMaxTime(arg1);
}

void MainWindow::on_ChipStart_clicked()
{
    on_B_PinConfig_Update_clicked();
    int configflags = 0;
    configflags += (true)?Configuration::dac:0;
    configflags += (true)?Configuration::config:0;
    configflags += (true)?Configuration::vdac:0;
    configflags += (true)?Configuration::colrow:0;

    if(!config.SendUpdate(configflags, true))
        logit("WriteASIC failed!");
    else
        logit("WriteASIC successful");

    //on_B_TDAC_Config_WriteMatrix_clicked();


    //on_B_TDAC_Config_WriteRow_clicked();



}

void MainWindow::on_actionLoad_Configuration_triggered()
{

}

void MainWindow::on_LogResetButton_clicked()
{
    this->ui->Log->clear();
}
