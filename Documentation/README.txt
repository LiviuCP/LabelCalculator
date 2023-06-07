0. INTRODUCTION

This application is used for creating a labelling table for cable connections between racked devices: servers, storage, switches, power distribution units etc. Both power and data connections are supported. One label is created for each connected port. The labels can be printed and attached to cables.

Note: I will use the term "management connections" for data connections related to device management. Many devices can have a dedicated Ethernet management port which is used for device administration. Additionally KVM connections could also be used for managing devices (usually servers).

1. DISCLAIMER

Although this application has been reasonably tested, it might still contain some bugs. Don't use it in any mission critical environments! I will take no responsibility for any damages it might cause whatsoever!

2. INSTALLATION AND SETUP

I am only providing this app as source code so a build needs to be performed in order to use it.

Following steps should be performed:
- download the codebase from the Git repository by previously selecting the LabelCalculator_v1.1 branch: https://github.com/LiviuCP/LabelCalculator.git
- open the project with Qt Creator or any other IDE supporting CMAKE and build it into any folder to which you have permissions

Notes:
- the application is written in standard C++ code, so no specific Qt deployment steps are required. Actually other frameworks might be used as well if they support CMAKE (minimum required version is 3.5).
- a compiler supporting C++17 is required.
- regarding Qt version I used 6.4.2 but other versions might be able to do the job as well.

3. SUPPORTED OPERATING SYSTEMS

The application has been compiled and tested on Linux (OpenSUSE 15.4), Mac OS (Monterey) and Windows 10.

4. RUNNING THE APPLICATION

Once the application has been compiled, launch the LabelCalculator executable from the build directory by using a terminal.

When running it, a menu opens from which the user will be requested to choose an option:
- option 1: create a labelling input template file based on defined rack connections (this should be filled-in by user and then used in option 2)
- option 2: create the labelling table based on the labelling input file
- option 3: create an empty connection definitions file that will subsequently be filled-in with the required data and then used in option 1

Before executing any of the first two options, the user should make sure a valid input file is available. This should be correctly filled-in and properly saved on the disk. The file could be:
- for option 1: connectiondefinitions.csv (defines connections between racked devices)
- for option 2: connectioninput.csv (contains detailed point-to-point connection input)

The input file should be named exactly as written above and placed within Documents/LabelCalculatorData subdirectory from the user home directory. Depending on the operating system, the path of the data directory would be:
- on Mac: /Users/[username]/Documents/LabelCalculatorData
- on Linux: /home/[username]/Documents/LabelCalculatorData
- on Windows: C:\Users\[username]\Documents\LabelCalculatorData

Once the input file has been created/updated within LabelCalculatorData, please enter the corresponding option from the keyboard and press ENTER. For exiting the application without performing any action simply press the return key (ENTER).

When the option (1 or 2) is executed, the application runs the necessary calculations on the data contained within input file. If the input is valid and no errors occured, an output file will be written to the same LabelCalculatorData folder.

Depending on the chosen option, the output file would be:
- connectioninput.csv for option 1
- labellingtable.csv for option 2

If parsing errors occur when processing the input file, these will be logged to an error file. The user will be requested to correct all errors by modifying the input file accordingly before re-running the application. The error file is also written to the LabelCalculatorData directory and is named error.txt. Only parsing errors are logged here (see section 6).

After correcting the parsing errors (and any other errors), the user should re-run the application, choose the option again and check the resulting output file upon successful completion. For more details see next sections.

The user can also choose option 3 for generating an empty connectiondefinitions.csv file. No input file is needed in this step. Then the user can fill in the required definition information and re-run the application, this time by choosing option 1.

To be noted:
 - the application only runs a single menu option and then exits. The user should reopen the application for executing the same option again or for running another menu option.
 - when running LabelCalculator, it is recommended to ensure that all files which reside in the LabelCalculatorData directory are not opened in another application, otherwise a file opening error could be triggered

5. DATA FILES AND DIRECTORIES

The LabelCalculatorData directory is created when running the application for the first time (even before the application menu is opened), unless it already exists in the Documents folder. When creating it, the application also includes following items:
- an ExampleFiles folder that contains application file examples. These have been copied from the build directory.
- a LabellingInputBackup folder (more details: see section 7)
- a LabellingOutputBackup folder (more details: see section 7)
- an empty connectiondefinitions.csv file (contains only the rack layout), same as the one created when running option 3

Regarding example files, following should be taken into consideration: each file contains a prefix that tags the connection type (data/management/power) and example number. This prefix should be removed if the user would like to run the application by providing the respective file as input. For example the file ex1_data_connectioninput.csv is the connectioninput.csv file belonging to Example 1, data connections table. The prefix "ex1_data_" should be removed if the user intends to feed this file to application option 2. For the template connection input files, also the substring "_template" should be removed and the file needs to be filled-in by replacing the placeholders with useful data in order to succesfully pass it to the application (more details regarding template connection input files: see sections 5.1, 5.2 and 8).

Following user data files are being processed or created by application:
- connection definitions file: connectiondefinitions.csv (input for option 1 / output for option 3)
- connection input file: connectioninput.csv (input for option 2 / output for option 1)
- labelling table file: labellingtable.csv (output for option 2)

In addition to these, an error file is created for either of the options 1 and 2. The name of the file is error.txt.

It is required that the exact naming of each input file is used and the file is placed within the LabelCalculatorData folder, otherwise an error will be triggered.

Both the labellingtable.csv and error.txt files can only be created by application. First one is generated when choosing option 2 (upon successful execution), while the other is used when for either of the first two options parsing errors occur. Please note that the error.txt file is automatically erased by application upon any successful execution (even when the user aborts by pressing ENTER instead of entering an option).

Notes:
- when errors which are not parser specific occur, they are displayed in detail within terminal and not in an error file. An example is the input file opening error. The user should find the cause of each error, correct it and re-run the application (more details see section 6)
- it is recommended to close all files residing in LabelCalculatorData before running the application

For more details regarding data files see the next sections.

5.1. CONNECTION DEFINITIONS

The connectiondefinitions.csv file is used when the user chooses option 1 in the application menu. The input contained within this file is processed by the application for creating a template connection input file (connectioninput.csv). The template will then be manually modified by user and subsequently fed to the application when executing option 2.

For obtaining a labelling table it is neither mandatory to run option 1 nor to have a connectiondefinitions.csv file. The user could create connectioninput.csv from scratch and then use it in option 2. However this step simplifies the creation of the connection input file and helps avoid potential parsing errors. It also assists in creating the labelling table in an orderly manner.

The connection definitions file has following structure:
- header row
- data rows which represent a rack layout

The header row should be present but is not used by application. It might be left empty as well without any impact on the outcome. It preceeds the data rows which start from row 2 in the spreadsheet.

The number of data rows is 50, one for each U position within rack. This corresponds to the configured maximum rack size (50U), which is enough for most racks. Any additional rows are ignored by application.

The data rows represent the rack layout starting at U50 and ending at U1. The data is structured as follows:
- the first column contains the U numbering of the rack. Its only purpose is to guide the user in defining the racked devices and connections but can be left empty as well.
- on the second column the device type can be defined for the corresponding U position. This is the first position (starting from bottom) occupied by the device within rack.
- starting with third column connections to the upper placed devices can be defined

To efficiently define device connections, the user should perform following steps:
- enter all required devices, one per row. This is done by filling in the type of each device on the second column on the lowest U position occupied by the device within rack.
- starting from the bottom of the rack, for each device the connected devices and the number of connection points should be defined. This is done by using the columns next to the device type. Each cell should contain the U position of the destination (target) device (to which the current device connects) and the number of connection points (cables).

For example, the server located at U5 might connect to two LAN switches, one located at U41 and the other at U42. The number of connection points to each switch is 2. Consequently following data should be entered:
- on the row corresponding to U5:
  - column 2 (device type): svr
  - column 3 (first connected device): 41/2
  - column 4 (second connected device): 42/2
- on the row corresponding to U41, column 2 (device type): lan
- on the row corresponding to U42, column 2 (device type): lan
- no connections to be entered on the LAN switch rows, these have been defined on the server row

Notes:
- for each device, only the devices located above it should be mentioned as connected devices
- the number of connection points should be higher than 0
- there should be no empty cells between device type and defined connected devices or between any two defined connected devices
- if the rack has a smaller size, the user should not fill in anything for the rows corresponding to the U positions not contained within rack. For example if the rack is 36U high, then the rows corresponding to positions 37U-50U should be left untouched.
- use positive integers for specifying U positions and keep the values within the allowed interval (1-50). For number of connections (connection points) positive integers higher than 0 should be applied. Avoid using any non-digit characters except the slash separating the target device U position from the number of connections. See more details in the provided examples.
- devices that occupy the same U position (e.g. left/right PDUs) should be entered as a single (consolidated) device and the number of connections should be increased if necessary. Then when running option 2 the point to point connections should be defined in detail by using the placement parameter as differentiator. For example if 2 PDUs are located at position 44 and connect to a device located at U50 (2 connections each) then they should be entered as a single PDU on the row corresponding to U44 and have 50/4 specified for the target device.
- please note that due to the format of the connected device entry (u_position/connections_count), the spreadsheet processing tool might do an automatic conversion to another format (typically date). If this happens, there is an easy workaround: enter an apostrophe character before the connected device string, e.g. '48/2. Ensure this character is visible in the cell (not in the formula bar). In Microsoft Excel for example it would be necessary to enter it twice (2 consecutive apostrophes - one will be visible in the cell) to make this happen (otherwise when closing the spreadsheet the character vanishes and when reopening it an automatic conversion might be performed). Please be advised that this workaround is not mandatory and is only available for this particular LabelCalculator data type (connected devices) only (for other cells the character is considered invalid by LabelCalculator). A single apostrophe should be used and this should be the first character in the connected device string. 
- an empty connection definitions file can be created by choosing the third option from the LabelCalculator menu. It only contains the rack layout and can be subsequently filled-in will all required devices and connections.

5.2. CONNECTION INPUT

This connectioninput.csv file is processed when the user chooses option 2 from the application menu. The contained input data is used for creating the final output, namely the labelling table (labellingtable.csv).

The connection input file consists of a:
- header row
- data rows each containing a connection between two device ports (point-to-point)

The header row should be present but is not used by the application. It might be left empty as well without impact on the outcome. The data rows (payload) start at row 2 in the spreadsheet.

Each data row contains a point-to-point cable connection between two power or data ports. The row data is structured as follows:
- cable part number
- type of the first device
- U number of the first device
- port parameters for first device
- type of the second device
- U number of the second device
- port parameters for second device

The device containing the first port on the row is considered the source device while the second port belongs to the destination (connected) device.

The required port parameters depend on the device type. For example a storage device demands following parameters to be entered in the exact order: controller number, port type and port number. Each parameter should be entered in a separate CSV cell starting with the cell next to the one containing the device U number.

It is mandatory to fill in all required port parameters for a specific device type. They should be entered in the right order and contiguously. A guideline for filling in connectioninput.csv can be found in the support file deviceportinput.csv (see Documentation directory).

In order to keep a correct alignment in the connection input table, a maximum allowed number of port parameters has been globally defined. Currently this number is 3 (the storage device port mentioned above has all of them). If any of the device ports has less input parameters, it is required that padding fields (cells) are used in order to reach this maximum number. These cells should stand next to the ones containing the filled-in parameters. They can be left empty or filled-in with any character sequence. Padding fields are mandatory only for the first device port on the row (source device port).

For example, if the device is a switch it only has the port type and port number parameters. If the switch port is the source port (first on the row), then one padding cell should be available after the one containing the port number. Next to this cell the port data of the second (destination) device is entered (starting with the device type). By doing this, it is ensured that the device type and U position of the second device are on the same columns with the device type and U position of each other destination device from the table. The padding cells are read by the parser but their content is discarded (thereby being allowed to fill in anything or nothing). However if no padding cells are entered, then the ones from the second device port are consumed as padding cells for the first one. This would lead to parsing errors.

Please note that the padding fields are automatically created when generating the connectioninput.csv file by executing option 1. They are being filled-in with the '-' character.

When generating the connectioninput.csv by using option 1, placeholders are being inserted for each port parameter and for the cable part number (PN). These should be filled-in by user with the actual parameter values (e.g. value 2 should replace the CONTROLLER NUMBER placeholder for a specific storage device data port). This is actually the highly recommended method of filling in the connection input file before handing it over to option 2.

Regarding cable part number, if the same PN is used on several contiguous rows, it is sufficient to fill it in for the first of these rows. All following rows with empty PN cell will have the same part number considered as input.

Last but not least, please be aware that for option 2 no check is being performed that the device ports contained on each row are actually connected (or belong to different devices as it should be), so take extra caution when creating the connection input file "from scratch". As mentioned before, it is highly recommended to create it as template by feeding connectiondefinitions.csv to option 1. Then the placeholders can be filled-in with the useful parameter values before launching option 2.

5.3. LABELLING TABLE

The labelling table is the final output of the application and can be obtained by providing a valid connection input file (connectioninput.csv), running the application and selecting the second option. It is written to labellingtable.csv.
The first table row is the header. All others are data rows.

Each table data row contains point-to-point cable connection data, namely:
- the cable part number
- description of the first (source) device port
- cable label for the first device port
- description of the second (destination) device port
- cable label for the second device port

There are two types of connections:
- power connections, e.g. server power supply to PDU
- data connections, e.g. storage FC port to SAN switch

Management connections are considered a sub-category of data connections and represent:
- connection of Ethernet management ports to management LAN switches
- connection of KVM ports to KVM switches

The connection type is not necessarily determined by device type, i.e. the PDU can also have a data connection between its management port and a LAN switch. On the other hand, each device has a power port that connects to a PDU, extension bar or UPS.

For better clarity and manageability, it is recommended to keep power, data and management connections in separate tables, i.e. run the whole end-to-end labelling table creation process once for each of these three scenarios.

5.4. ERROR FILE

The error.txt file contains any parsing errors that prevent the output file from being created. This only applies when running either option 1 or 2.

For each row of the connection definitions/input file (see sections 5.1/5.2), one or more parsing errors would occur if the input data does not fulfill certain requirements. Each encountered parsing error is logged within error.txt file in the order of its discovery.

The logging entry contains:
- error code
- error description
- CSV row and (in most cases) column where the error occured (or for file-level errors the name and path of the input file)
- corrective actions from user side

The CSV row/column where the error fired can be identified by opening the input file in a spreadsheet processing tool. The column number mentioned in the error file corresponds to the column letter displayed by the spreadsheet tool, e.g. column number 8 mentioned in error.txt is column H as displayed by the spreadsheet app. The row number is the exact one mentioned in the spreadsheet. Row numbering starts at 2 as number 1 is reserved for the header.

Notes:
- each time the application opens, an empty error file is created or if one already exists its content is erased. Then if any parsing errors occur, the error file is updated by logging them into it. No output file is written to the LabelCalculatorData directory. On the other hand, if no parsing errors were triggered, then the file would be erased before the application exits. This includes the scenario when the user just hits ENTER instead of entering an option and acknowledging it.
- if the user forces the application to quit (by hitting CTRL+C or closing the window) an empty error.txt file remains in the LabelCalculatorData directory.

6. ERRORS

There are more types of errors:
- parsing errors
- labelling errors
- other errors

6.1. PARSING ERRORS

Parsing errors lead to application termination without any usable output written to file. The errors are written to error.txt. The parsing errors that might be encountered by user and the corresponding corrective actions are mentioned in the guidelines contained in the support file parsingerrors.csv.

6.2. LABELLING ERRORS

Labelling errors only impact the device ports for which they occurred. They are triggered for option 2 if no hard parsing errors had previously occurred. These errors are written to the labellingtable.csv output file on the appropriate rows, namely for the ports for which they were triggered. For the other rows/ports where the input is error-free the output is written as expected into the same file. Each labelling error is filled-in into labellingtable.csv within the description and label cells of the device port for which it occurred. It might also be filled-in into the cable PN field if it relates to the missing/invalid cable part number. An example of labelling error is the one that occurs when the PDU load segment number is invalid, meaning it hasn't been entered as a positive integer by user.

The program still displays a successful completion in the command line interface when these errors occur. Consequently, the user should check the labelling table and correct the connectioninput.csv data if needed or manually overwrite the affected entries if a custom labelling is required for them.

Note: a labelling error that might occur for any device port type is the one regarding exceeding the maximum number of characters that are allowed within label (currently this is 16). If the label calculated based on device port parameter values entered by user exceeds this threshold, then appropriate error texts will replace the description and label of the affected port in the labelling table. The description text mentions the actual error and corrective actions (reduce the number of characters from the input), while the label text mentions the number of characters above the limit. The user should update the device port parameters (the ones that support variable characters count, like port number) in the connectioninput.csv file so this threshold is no longer exceeded. Then option 2 should be run again on the updated file.

For more information regarding labelling errors please check labellingerrors.csv.

6.3 OTHER ERRORS

Other possible errors are:
- unable to open the input file for reading (when executing option 1 or 2): file might be missing or the user is lacking required permissions
- unable to open the output file for writing (for any of the available options): same possible reasons or the file might exist and already be open in another application (the last reason typically occurs on Windows)
- unable to open the error file for writing (for any of the first two options): same possible reasons as for output file
- setup error, user not recognized etc.

When these errors occur, please make sure:
- your user account is correctly setup
- the required files exist and reside in the correct directory (Documents/LabelCalculatorData).
- you have the right read/write/execute permissions for them
- ensure the files are not open in another application when running LabelCalculator

Note: these errors are not logged in any file but instead are displayed within command line interface (CLI).

7. BACKUP

When the user chooses to perform one of the available operations and a data file is about to be overwritten, a backup of this file is being performed by moving it to a backup directory. A timestamp representing the current date and time is also prepended to the moved item.

The timestamp has two purposes:
- to prevent overriding another existing file from the backup folder
- it also helps the user to trace the previously performed work easier

Depending on the role of the file in the labelling table creation process, the move is being performed to the input backup or output backup folder.

Notes:
- the backup is performed regardless whether the operation requested by user is successful or not. The application backs up the item prior to starting any concrete operation.
- the application does not check the content of the backed up item. For example if the user runs option 3 multiple times without touching the connectiondefinitions.csv file that exists in the LabelCalculatorData directory, then a backup of the empty file (rack layout only) will be run at least two times (first time the file might have not been empty).

7.1. INPUT FILE BACKUP

The input file backup concerns the connectiondefinitions.csv and connectioninput.csv files. It is being performed when the user executes one of the options 1 and 3. If any of these two files is about to be overwritten it is first renamed and moved to the LabellingInputBackup directory prior to starting the actual operation requested by user.

7.2. OUTPUT FILE BACKUP

The output file backup concerns the labellingtable.csv file. It is being performed when the user chooses option 2, provided that a labellingtable.csv file already exists in the LabelCalculatorData directory. If this is the case, the file is renamed and moved to the LabellingOutputBackup directory before starting the compute operations for a new labelling table.

8. MISCELLANEOUS

Currently the application does not support inter-rack connectivity. Also it doesn't check duplicate point-to-point connections (same port connected to same port on two different table rows) or port-to-port connection validity (e.g. invalid connection between a FC port and a PDU power port). All these might be added in a future version.

For a better understanding of the application options and files, please also check the other documents from the Documentation folder. You will find examples of data, power and management connections. All relevant input or output files except error.txt are provided there. Please note that for all connection types from all examples the substring "_template" has been manually added to template connection input files to reflect the fact that they are generated when executing option 1 and each file is only a template that needs to be filled-in prior to executing the second option. More details about this in section 5.
