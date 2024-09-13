# 3004TermProject

## Team members: 
- Shreya Voore
- Nguyen-Hanh Nong
- Ashok Sivathayalan
- Awwab Mahdi

## Includes:

.
├── 3004ProjTest (folder containing the code/implementation of the term project)/
│   ├── 3004ProjTest.pro (the file needed for QTCreator to run)
│   ├── 3004ProjTest.pro.user (another file needed for QTCreator to run)
│   ├── 3004ProjTest.qrc (file that tells QTCreator about non-source/non-header files)
│   ├── battery.cpp (source file for Battery class)
│   ├── battery.h (header file for Battery class)
│   ├── high_coherence_scores.txt (txt file containing the coherence scores for the high coherence data)
│   ├── led.cpp (source file for LED class)
│   ├── led.h (header file for LED class)
│   ├── lightstrip.cpp (source file for LightStrip class)
│   ├── lightstrip.h (header file for LightStrip class)
│   ├── low_coherence_data.txt (txt file containing low coherence data)
│   ├── low_coherence_scores.txt (txt file containing the coherence scores for the low coherence data)
│   ├── main.cpp (main file for project)
│   ├── mainwindow.cpp (source file for MainWindow class)
│   ├── mainwindow.h (header file for MainWindow class)
│   ├── mainwindow.ui (file containing UI for the program)
│   ├── medium_coherence_data.txt (txt file containing medium coherence data)
│   ├── medium_coherence_scores.txt (txt file containing the coherence scores for the medium coherence data)
│   ├── menu.cpp (source file for Menu class)
│   ├── menu.h (header file for Menu class)
│   ├── qcustomplot.cpp (source file for QCustomPlot class)
│   ├── qcustomplot.h (header file for QCustomPlot class)
│   ├── reader.cpp (source file for Reader class)
│   ├── reader.h (header file for Reader class)
│   ├── session.cpp (source file for Session class)
│   └── session.h (header file for Session class)
├── Design Documentation (folder containing our various design documentation and traceability matrix)/
│   ├── Device_Screens.pdf (PDF containing our initial designs/drawings for the UI)
│   ├── Sequence Diagrams (folder containing the 3 sequence diagrams)/
│   │   ├── Battery-Drained-Sequence-Diagram.png (battery drained sequence diagram)
│   │   ├── Normal-Scenario-Sequence-Diagram.png (normal operation sequence diagram)
│   │   └── Sensor-Interruption-Sequence-Diagram.png (interruption due to sensor off sequence diagram)
│   ├── Traceability_Matrix.pdf (PDf containing traceability matrix)
│   ├── UML_Class_Diagram.png (image containing our UML class diagram)
│   ├── Use_Cases.pdf (PDf containing our use cases)
│   └── Use_Case_Diagram.png (image of our use case diagram)
└── README.md (readme file for project)

## Video Link:
- https://youtu.be/spAv53TgZIQ

## Contributions:
- Shreya Voore:
  - Developed the Menu class used in the application.
  - Created the use case diagram and battery low sequence diagram.
  - Primarily developed the GUI displayed in the program and worked on integrating the various components of the program together in the MainWindow class.
- Nguyen-Hanh Nong:
  - Developed the Reader class used in the application.
  - Created the normal operation sequence diagram.
  - Added functionality to add and remove menus in the GUI.
  - Added graphing functionality to the MainWindow class and dealt with data plotting and data creation.
- Ashok Sivathayalan:
  - Developed the Battery, LED, Lightstrip classes used in the application.
  - Created the UML class diagram and developed the Demo Video
  - Integrated the Breath pacer lights, heart rate sensor (on off sensor) and breath pacer slider into the UI in the MainWindow class.
- Awwab Mahdi:
  - Developed the Session class used in the application.
  - Created the Interruption due to sensor off Sequence Diagram and traceability matrix.
  - Developed several use cases including the main use case.
