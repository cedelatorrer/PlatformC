// Eliminar esta linea en caso de que no falle el programa // int Lv_Corte  = 46;
// Variables de resultado y entrada
//int Lv_Leer    = 45; // Variable de salida, conectada a la tarjeta DIO96, da aviso sobre si el programa se encuentra ejecutandose o está disponible para empezar.
//int Lv_Result = 46; // Variable de salida, conectada a la tarjeta DIO96, envia el resultado del proceso de insercion.

//señale (INPUT)
//int Lv_Senal  = 44; // Se asocia a la variable Estado_Senal , inicia la insercion automatica

//Se Cambian a Variables/////////////////////// 4 de marzo del 2024
//int DebugSignal   = 0; // Variable DebugSignal, recibida a través de Mensaje serial, sirve para activar la funcion debug
int MainSignal    = 1; // Variable MainSignal, recibida a través de Mensaje serial, sirve para activar la variable Main al tener activa la función debug
int ClinchSignal  = 1; // Variable ClinchSignal, recibida a través de Mensaje serial, sirve para activar la variable Clinch al tener activa la función debug
int GuiaSignal    = 0; // Variable GuiaSignal, recibida a través de Mensaje serial, sirve para activar la variable Guia al tener activa la función debug
int InsertSignal  = 0; // Variable InsertSignal, recibida a través de Mensaje serial, sirve para activar la variable Insert al tener activa la función debug
int CutSignal     = 0; // Variable CutSignal, recibida a través de Mensaje serial, sirve para activar la variable Cut al tener activa la función debug
//Variable para el funcionamiento del clinch
int Estado_ClinchCtl  = 1;

//Relevadores de valvulas (OUTPUTS)
int Main    = 30; // Variable Main, conectada al modulo de relevadores para activar la valvula principal de aire.
int Clinch  = 29; // Variable Clinch, conectada al modulo de relevadores para activar la valvula que activa el piston que realiza el clinch del tab
int Guia    = 28; // Variable Guia, conectada al modulo de relevadores para activar la valvula que activa el piston de la guia
int Insert  = 27; // Variable Insert, conectada al modulo de relevadores para activar la valvula que activa el piston de insercion del tab
int Cut     = 26; // Variable Cut, conectada al modulo de relevadores para activar la valvula que activa el piston de corte.

// Sensores (INPUTS)
int Guide_up  = 38; // Variable Guide_Up, asociada al sensor del vastago de guia
int Guide_down  = 33; // Variable Guide_Up, asociada al sensor del vastago de guia
int Clinch_up   = 35; // Variable Clinch_Up, asociada al sensor del piston que realiza el clinch
int Clinch_down  = 32; // Variable Clinch_Up, asociada al sensor del piston que realiza el clinch
int Insert_down = 34; // Variable Insert_Down, asociada al sensor del piston de insercion cuando este se encuentra abajo
int Insert_up   = 36; // Variable Insert_up, asociada al sensor del piston de insercion cuando este se encuentra arriba (Este sensor no esta funcionando)
int Cut_close   = 37; // Variable Cut close, asociada al sensor del piston de corte cuando este se encuentra cerrado
int Cut_open    = 31; // Variable Cut open, asociada al sensor del piston de corte cuando este se encuentra abierto

//Variables de los sensores
int Estado_CutOpen    = 0;
int Estado_CutClose   = 0;
int Estado_InsertUp   = 0;
int Estado_InsertDown = 0;
int Estado_ClinchUp   = 0;
int Estado_ClinchDown = 0;
int Estado_GuideUp  = 0;
int Estado_GuideDown  = 0;
int Estado_Senal      = 0;
int Contador          = 0;


String MensajeLabview = "";
bool stringComplete = false;

//timers
unsigned long startmillis = 0;
unsigned long endmillis = 0;


void setup() {
  // put your setup code here, to run once:

  //Señales de los sensores
  pinMode (Cut_open, INPUT);
  pinMode (Cut_close, INPUT);
  pinMode (Insert_up, INPUT);
  pinMode (Insert_down, INPUT);
  pinMode (Clinch_up, INPUT);
  pinMode (Clinch_down, INPUT);
  pinMode (Guide_down, INPUT);
  pinMode (Guide_up, INPUT);
  //pinMode (Lv_Senal, INPUT); Tal vez se elimine
  // Eliminar esta linea en caso de que no falle el programa // pinMode (Lv_Corte,INPUT);

  /*//LV señales recibidas de labview (Cambiadas a comunicacion serial: 4 de marzo del 2024)
    pinMode (MainSignal, INPUT);
    pinMode (CutSignal, INPUT);
    pinMode (GuiaSignal, INPUT);
    pinMode (InsertSignal, INPUT);
    pinMode (ClinchSignal, INPUT);
    pinMode (DebugSignal, INPUT);
    pinMode (ClinchControl, INPUT);
    ///////////////////////////////////////////////////////////////////////////////////////////////*/


  //Salidas a relevadores
  pinMode (Cut, OUTPUT);
  pinMode (Guia, OUTPUT);
  pinMode (Insert, OUTPUT);
  pinMode (Clinch, OUTPUT);
  pinMode (Main, OUTPUT);

  //Salidas a LV
  //pinMode (Lv_Leer, OUTPUT);
  //pinMode (Lv_Result, OUTPUT);

  //Estado de los relevadores
  digitalWrite(Main, LOW);
  digitalWrite(Insert, HIGH);
  digitalWrite(Guia, HIGH);
  digitalWrite(Clinch, HIGH);
  digitalWrite(Cut, HIGH);
  //digitalWrite(Lv_Result, HIGH);
  //digitalWrite(Lv_Leer, HIGH);

  Serial.begin(115200);
  while (!Serial);
  Serial.println("Ready!");
}


void loop() {


  // put your mai<n code here, to run repeatedly:

  /////////////Lectura de los pines de entrada///////////////

  //if (Serial.available() != 0) {
  // LecturaSerial();
  // }



  Estado_CutOpen    =    digitalRead(Cut_open); //Sensor
  Estado_CutClose   =   digitalRead(Cut_close); //Sensor
  Estado_InsertUp   =   digitalRead(Insert_up); //Sensor
  Estado_InsertDown = digitalRead(Insert_down); //Sensor
  Estado_ClinchUp   =   digitalRead(Clinch_up); //Sensor
  Estado_ClinchDown   =   digitalRead(Clinch_down); //Sensor
  Estado_GuideDown =    digitalRead(Guide_down); //Sensor
  Estado_GuideUp  =    digitalRead(Guide_up); //Sensor


if(stringComplete)
    {
 ///////////////////////////////////////////////////////////////////////////////////////////Movimientos manuales a traves de LabVIEW/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 //////////////////////////////////////////////////////////////////////Desactivacion del modo manual///////////////////////////////////////////////////////////////////////
  if (MensajeLabview == "Debug,0") {
    DebugSignal = 0;
    Serial.println("OK");
  }
  //////////////////////////////////////////////////////////////////////Activacion del modo manual///////////////////////////////////////////////////////////////////////
  else if (MensajeLabview == "Debug,1") {
    DebugSignal = 1;
    Serial.println("OK");
  }
*/
  //////////////////////////////////////////////////////////////////////Desactivacion el control del clinch///////////////////////////////////////////////////////////////////////
  if (MensajeLabview == "ClinchCtl,0") {
    Estado_ClinchCtl = 0;
    Serial.println("OK");
  }
  //////////////////////////////////////////////////////////////////////Activacion el control del clinch///////////////////////////////////////////////////////////////////////
  else if (MensajeLabview == "ClinchCtl,1") {
    Estado_ClinchCtl = 1;
    Serial.println("OK");
  }
  //////////////////////////////////////////////////////////////////////Desactivacion de valvula principal de entada de aire///////////////////////////////////////////////////////////////////////
  else if (MensajeLabview == "Main,0") {
    MainSignal = 0;
    if (MainSignal == 0) {
      digitalWrite(Main, HIGH); //Desactiva la valvula principal de aire
      Serial.println("OK");
    }
    else {
      Serial.println("ERROR, Debug Off");
    }
  }
  //////////////////////////////////////////////////////////////////////Activacion de valvula principal de entada de aire///////////////////////////////////////////////////////////////////////
  else if (MensajeLabview == "Main,1") {
    MainSignal = 1;

    if (MainSignal == 1) {
      digitalWrite(Main, LOW); // Activa la valvula principal de aire
      Serial.println("OK");
    }
    else {
      Serial.println("ERROR, Debug Off");
    }
  }
  //////////////////////////////////////////////////////////////////////Desactivacion de valvula insert///////////////////////////////////////////////////////////////////////
  else if (MensajeLabview == "Insert,0") {
    InsertSignal = 0;
    if (InsertSignal == 0) {
      digitalWrite(Insert, HIGH); //Desactiva la valvula que controla la la insercion del tab
      Serial.println("OK");
    }
    else {
      Serial.println("ERROR, Debug Off");
    }
  }
  //////////////////////////////////////////////////////////////////////Activacion de valvula insert///////////////////////////////////////////////////////////////////////
  else if (MensajeLabview == "Insert,1") {
    InsertSignal = 1;
    if (InsertSignal == 1
        && CutSignal == 0) {
      digitalWrite(Insert, LOW); //Activa la valvula que controla la la insercion del tab
      Serial.println("OK");
    }
    else if (InsertSignal == 1
             && CutSignal == 1) {
      Serial.println("ERROR, la valvula que corta el tab se encuentra activa");
    }
    else {
      Serial.println("ERROR, Debug Off");
    }
  }

  //////////////////////////////////////////////////////////////////////Desactivacion de valvula de la Guía (Pin)///////////////////////////////////////////////////////////////////////
  else if (MensajeLabview == "Guia,0") {
    GuiaSignal = 0;
    if (GuiaSignal == 0) {
      digitalWrite(Guia, HIGH);  //Desactiva la valvula que controla la guia del tab
      Serial.println("OK");
    }
    else {
      Serial.println("ERROR, Debug Off");
    }
  }
  //////////////////////////////////////////////////////////////////////Activacion de valvula de la Guía (Pin)///////////////////////////////////////////////////////////////////////
  else if (MensajeLabview == "Guia,1") {
    GuiaSignal = 1;
    if (GuiaSignal == 1) {
      digitalWrite(Guia, LOW); //Activa la valvula que controla la guia del tab
      Serial.println("OK");
    }
    else {
      Serial.println("ERROR, Debug Off");
    }
  }
  //////////////////////////////////////////////////////////////////////Desactivacion del clinch///////////////////////////////////////////////////////////////////////
  else if (MensajeLabview == "Clinch,0") {
    ClinchSignal = 0;
    if (ClinchSignal == 0)
    {
      digitalWrite(Clinch, HIGH); //Desactiva la valvula que controla el clinch
      Serial.println("OK");
    }
    else {
      Serial.println("ERROR, Debug Off");
    }
  }
  //////////////////////////////////////////////////////////////////////Activacion del clinch///////////////////////////////////////////////////////////////////////
  else if (MensajeLabview == "Clinch,1") {
    ClinchSignal = 1;
    if (ClinchSignal == 1
        && Estado_ClinchCtl == 1)
    {
      digitalWrite(Clinch, LOW);  //Activa la valvula que controla el clinch
      Serial.println("OK");
    }
    else if (ClinchSignal == 1
             && Estado_ClinchCtl == 0)
    {
      Serial.println("ERROR, El control del Clinch se encuentra desactivado");
    }
    else {
      Serial.println("ERROR, Debug Off");
    }
  }
  //////////////////////////////////////////////////////////////////////Desactivacion de valvula que corta el tab///////////////////////////////////////////////////////////////////////
  else if (MensajeLabview == "Cut,0") {
    CutSignal = 0;
    if (CutSignal == 0) {
      digitalWrite(Cut, HIGH); //Desactiva la valvula para cortar el tab
      Serial.println("OK");
    }
    else {
      Serial.println("ERROR, Debug Off");
    }
  }
  //////////////////////////////////////////////////////////////////////Activacion de valvula que corta el tab///////////////////////////////////////////////////////////////////////
  else if (MensajeLabview == "Cut,1") {
    CutSignal = 1;
    if (CutSignal == 1
        && InsertSignal == 0) {
      digitalWrite(Cut, LOW); //Activa la valvula para cortar el tab
      Serial.println("OK");
    }
    else if (CutSignal == 1
             && InsertSignal == 1) {
      Serial.println("ERROR, el insert se encuentra activo");
    }
    else {
      Serial.println("ERROR, Debug Off");
    }
  }

  //////////////////////////////////////////////////////////////////////Mensaje para insercion automatica///////////////////////////////////////////////////////////////////////
  else if (MensajeLabview == "Start") {
    Estado_Senal = 1;
    digitalWrite(Main, LOW);
    digitalWrite(Insert, HIGH);
    digitalWrite(Guia, HIGH);
    digitalWrite(Clinch, HIGH);
    digitalWrite(Cut, HIGH);
  }
    else if (MensajeLabview == "RESET") {
      

    }
  //////////////////////////////////////////////////////////////////////Mensaje cuando no recibe nada////////////////////////////////////////////////////////////////////////////
   else{
    Serial.println("ERROR, No se reconoce el comando");
   }
    MensajeLabview = "";
    stringComplete = false;
 }
////////////////////////////////////////////////////////Inicia la insercion, activa insert y guia(Insert baja y guia sube)///////////////////////////////////////////////////////////////////////////////////////////////////////

  if (Estado_InsertUp == 0
      && Estado_InsertDown == 1
      && Estado_ClinchDown == 0
      && Estado_ClinchUp == 1
      && Estado_CutOpen == 0
      && Estado_CutClose == 1
      && Estado_GuideUp == 1
      && Estado_GuideDown == 0
      && Estado_Senal == 1   //**
      && Contador == 0)
  {
    Estado_Senal = 0;
    startmillis = millis();
    Contador = 1;
    //digitalWrite(Lv_Result, HIGH);
    //digitalWrite(Lv_Leer, HIGH);
    digitalWrite(Main, LOW);
    digitalWrite(Insert, LOW);
    digitalWrite(Guia, LOW);
  }
  ///////////////// verifica que si ha bajado el insert y la guia y sube el clinch Contador =1///////////////////// Contador =1 Control de clinch=1(activo)  Clinch: Sube

  else if (Estado_InsertDown == 0
           && Estado_InsertUp == 1
           && Estado_GuideUp == 0
           && Estado_GuideDown == 1
           && Estado_ClinchUp == 1
           && Estado_ClinchDown == 0
           && Estado_CutOpen == 0
           && Estado_CutClose == 1
           && Contador == 1
           && Estado_ClinchCtl == 1)
  {
    startmillis = millis();
    Contador = 2;
    digitalWrite(Clinch, LOW);
    delay(13);
  }

  ///////////////// verifica que si ha bajado el insert y la guia y sube el clinch Contador:1.2 ///////////////////// Contador =1 Control de clinch=1(activo)  Clinch: Sube

  else if (Estado_InsertDown == 0
           && Estado_InsertUp == 1
           && Estado_GuideUp == 0
           && Estado_GuideDown == 1
           && Estado_ClinchUp == 1
           && Estado_ClinchDown == 0
           && Estado_CutOpen == 0
           && Estado_CutClose == 1
           && Contador == 1
           && Estado_ClinchCtl == 0)
  {
    startmillis = millis();
    Contador = 2;
    digitalWrite(Clinch, HIGH);
    delay(13);
  }

  /////////Verifica que ha subido el clinch, envia la guia, insert y clinch a su inicio Contador:2.1/////////////// Contador =2 Control de clinch=1(activo)  Clinch: Regresa a posicion inicial

  else if (Estado_InsertDown == 0
           && Estado_InsertUp == 1
           && Estado_GuideUp == 0
           && Estado_GuideDown == 1
           && Estado_ClinchUp == 0
           && Estado_ClinchDown == 1
           && Estado_CutOpen == 0
           && Estado_CutClose == 1
           && Contador == 2
           && Estado_ClinchCtl == 1)
  {
    startmillis = millis();
    Contador = 3;
    digitalWrite(Insert, HIGH);
    digitalWrite(Guia, HIGH);
    digitalWrite(Clinch, HIGH);
    //digitalWrite(Lv_Result, LOW);
    //digitalWrite(Lv_Leer, LOW);
  }

  /////////Verifica que ha subido el clinch, envia la guia, insert y clinch a su inicio Contador:2.2/////////////// Contador =2 Control de clinch=1(activo)  Clinch: Regresa a posicion inicial

  else if (Estado_InsertDown == 0
           && Estado_InsertUp == 1
           && Estado_GuideUp == 0
           && Estado_GuideDown == 1
           && Estado_ClinchUp == 1
           && Estado_ClinchDown == 0
           && Estado_CutOpen == 0
           && Estado_CutClose == 1
           && Contador == 2
           && Estado_ClinchCtl == 0)
  {
    startmillis = millis();
    Contador = 3;
    digitalWrite(Insert, HIGH);
    digitalWrite(Guia, HIGH);
    digitalWrite(Clinch, HIGH);
    //digitalWrite(Lv_Result, LOW);
    //digitalWrite(Lv_Leer, LOW);
  }

  /////////////Detecta que se envie la señal por parte de labview y envia la señal de regreso a labview////////

  else if (Contador == 3)
  {
    startmillis = millis();
    Contador = 4;
    Serial.println("OK");
  }
  ////////////Activa el piston de corte//////////////////

  else if (Estado_InsertDown == 1
           && Estado_InsertUp == 0
           && Estado_GuideUp == 1
           && Estado_GuideDown == 0
           && Estado_ClinchUp == 1
           && Estado_ClinchDown == 0
           && Estado_CutOpen == 0
           && Estado_CutClose == 1
           && Contador == 4)
  {
    startmillis = millis();
    digitalWrite(Cut, LOW);
    Contador = 5;
  }
  ////////////Desactiva el piston de corte//////////////

  else if (Estado_InsertDown == 1
           && Estado_InsertUp == 0
           && Estado_GuideUp == 1
           && Estado_GuideDown == 0
           && Estado_ClinchUp == 1
           && Estado_ClinchDown == 0
           && Estado_CutOpen == 1
           && Estado_CutClose == 0
           && Contador == 5)
  {
    startmillis = millis();
    digitalWrite(Cut, HIGH);
    //digitalWrite(Main,HIGH);
    Contador = 6;
  }
  ////////////////////Verifica que corto el tab///////////////

  else if (Estado_InsertDown == 1
           && Estado_InsertUp == 0
           && Estado_GuideUp == 1
           && Estado_GuideDown == 0
           && Estado_ClinchUp == 1
           && Estado_ClinchDown == 0
           && Estado_CutOpen == 0
           && Estado_CutClose == 1
           && Contador == 6)
  {
    Contador = 0;
  }
  ///////inicia el conteo para enviar la ERROR al programa////////

  endmillis = millis();


  /////////////Envia la ERROR al programa/////////////

  if (endmillis - startmillis > 1000
      && Contador > 0)
  {


    ////////////////Envio de ERROR por puerto serial a labview/////////////////////////
    if (Contador == 1)
    {
      if (Estado_InsertDown == 1)
      {
        Serial.println("ERROR, el sensor que detecta el piston de insercion cuando esta abajo, no se activó. Revisar que el piston ha bajado o que el sensor no se encuentra fuera de su posicion.");
      }
      if (Estado_InsertUp == 0)
      {
        Serial.println("ERROR, el sensor que detecta el piston de insercion cuando esta arriba, sigue activo. Revisar el piston de insercion.");
      }
      if (Estado_ClinchUp == 0)
      {
        Serial.println("ERROR, el sensor que detecta el piston de clinch , detectó que el piston está activado. Revisar la posicion del sensor y que el piston no se encuentre activo.");
      }
      if (Estado_ClinchCtl == 0)
      {
        Serial.println("ERROR, clinchCtrl no se encuentra activo (señal de Labview)");
      }
      Contador = 0;
      digitalWrite(Insert, HIGH);
      digitalWrite(Guia, HIGH);
      digitalWrite(Clinch, HIGH);
      //digitalWrite(Lv_Result, HIGH);
      //digitalWrite(Lv_Leer, HIGH);
      endmillis = startmillis;
    }



    else if (Contador == 2) {
      if (Estado_ClinchUp == 1)
      {
        Serial.println("ERROR, No se activo el sensor del clinch, revise que el sensor se encuentra ajustado o que si se haya activado el piston del clinch");
      }
      if (Estado_ClinchCtl == 0)
      {
        Serial.println("ERROR, clinchCtrl no se encuentra activo (señal de Labview)");
      }
      Contador = 0;
      digitalWrite(Insert, HIGH);
      digitalWrite(Guia, HIGH);
      digitalWrite(Clinch, HIGH);
     //digitalWrite(Lv_Result, HIGH);
      //digitalWrite(Lv_Leer, HIGH);
      endmillis = startmillis;
    }

    else if (Contador == 3) {
      
    }
    else if (Contador == 4) {
      if (Estado_CutOpen == 1)
      {
        Serial.println("ERROR, el sensor que detecta el piston de corte cuando esta abierto, detectó que el piston esta cerrado. Revisar que el sensor se encuentre en posicion y que el piston de corte se encuentre abierto.");
      }

      if (Estado_InsertUp == 1)
      {
        Serial.println("ERROR, el sensor que detecta el piston de insercion cuando esta arriba, detectó que el piston sigue abajo. Revisar posicion del sensor y la posicion del piston. ");
      }

      if (Estado_InsertDown == 0)
      {
        Serial.println("ERROR, el sensor que detecta el piston de insercion cuando esta abajo, detectó que el piston sigue abajo. Revisar la posicion del piston y que el sensor se encuentre en su posicion.");
      }

      if (Estado_ClinchUp == 0)
      {
        Serial.println("ERROR, el sensor que detecta el piston de clinch , detectó que el piston está activado. Revisar la posicion del sensor y que el piston no se encuentre activo.");
      }
      Contador = 0;
      digitalWrite(Insert, HIGH);
      digitalWrite(Guia, HIGH);
      digitalWrite(Clinch, HIGH);
      //digitalWrite(Lv_Result, HIGH);
      //digitalWrite(Lv_Leer, HIGH);
      endmillis = startmillis;
    }

    else if (Contador == 5) {
      if (Estado_CutClose == 1)
      {
        Serial.println("ERROR, el sensor que detecta el piston de corte cuando esta cerrado, detectó que el piston esta abierto. Revisar la posicion del sensor y la posicion del piston");
      }

      if (Estado_InsertUp == 1)
      {
        Serial.println("ERROR, el sensor que detecta el piston de insercion cuando esta arriba, detectó que el piston sigue abajo. Revisar posicion del sensor y la posicion del piston. ");
      }

      if (Estado_InsertDown == 0)
      {
        Serial.println("ERROR, el sensor que detecta el piston de insercion cuando esta abajo, detectó que el piston sigue abajo. Revisar la posicion del piston y que el sensor se encuentre en su posicion.");
      }

      if (Estado_ClinchUp == 0)
      {
        Serial.println("ERROR, el sensor que detecta el piston de clinch , detectó que el piston está activado. Revisar la posicion del sensor y que el piston no se encuentre activo.");
      }
      ////////////////Regreso de pistones a estado inicial/////////////////////////
      Contador = 0;
      digitalWrite(Insert, HIGH);
      digitalWrite(Guia, HIGH);
      digitalWrite(Clinch, HIGH);
      //digitalWrite(Lv_Result, HIGH);
      //digitalWrite(Lv_Leer, HIGH);
      endmillis = startmillis;
    }

    else if (Contador == 6) {
      if (Estado_CutClose == 0)
      {
        Serial.println("ERROR, el sensor que detecta el piston de corte cuando esta cerrado, detectó que el piston sigue cerrado. Revisar la posicion del sensor y el piston");
      }

      if (Estado_CutOpen == 1)
      {
        Serial.println("ERROR, el sensor que detecta el piston de corte cuando esta abierto, detectó que el piston sigue cerrado. Revisar la posicion del sensor y el piston");
      }

      if (Estado_InsertUp == 1)
      {
        Serial.println("ERROR, el sensor que detecta el piston de insercion cuando esta arriba, detectó que el piston sigue abajo. Revisar posicion del sensor y la posicion del piston. ");
      }

      if (Estado_InsertDown == 0)
      {
        Serial.println("ERROR, el sensor que detecta el piston de insercion cuando esta abajo, detectó que el piston sigue abajo. Revisar la posicion del piston y que el sensor se encuentre en su posicion.");
      }

      if (Estado_ClinchUp == 0)
      {
        Serial.println("ERROR, el sensor que detecta el piston de clinch , detectó que el piston está activado. Revisar la posicion del sensor y que el piston no se encuentre activo.");
      }
      Contador = 0;
      digitalWrite(Insert, HIGH);
      digitalWrite(Guia, HIGH);
      digitalWrite(Clinch, HIGH);
     //digitalWrite(Lv_Result, HIGH);
     // digitalWrite(Lv_Leer, HIGH);
      endmillis = startmillis;
    }

    ////////////////Regreso de pistones a estado inicial/////////////////////////
    Contador = 0;
    digitalWrite(Insert, HIGH);
    digitalWrite(Guia, HIGH);
    digitalWrite(Clinch, HIGH);
    //digitalWrite(Lv_Result, HIGH);
    //digitalWrite(Lv_Leer, HIGH);
    endmillis = startmillis;
  }



}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();  //Get the new byte:
    if (inChar == '\n')
      stringComplete = true; //If the incoming character is a newline, set a flag so the main loop
    else
      MensajeLabview += inChar; //Add it to the inputString
  }
}
