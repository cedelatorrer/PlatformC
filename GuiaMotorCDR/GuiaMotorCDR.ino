/* //////////////////////////////////////// PINOUT //////////////////////////////////////// */
const int pinStepX  = 2;
const int pinDirX   = 3;
const int pinStepY  = 4;
const int pinDirY   = 5;

const int pinEnableX   = 6;
const int pinEnableY   = 7;

const int pinEmergencyStop   = 21;
const int pinErrX   = 18;
const int pinErrY   = 19;

const int pinHomeX  = 22;
const int pinLimX   = 23;
const int pinHomeY  = 25;
const int pinLimY   = 24;

/* //////////////////////////////////////// FLAGS //////////////////////////////////////// */
bool EmergencyStop;
bool ErrX;
bool ErrY;
bool DetenerButton;
bool HomeRequired;

/* //////////////////////////////////////// CONSTANTES //////////////////////////////////////// */
const int MOTORX  = 0;
const int MOTORY  = 1;
const int ENABLE  = 0;
const int DISABLE = 1;
const int LEFT    = 0;
const int RIGHT   = 1;


void(* resetFunc) (void) = 0; //Reset function @ address 0

String inputString = "";      //String to hold incoming data
bool stringComplete = false;  //Whether the string is complete

/* //////////////////////////////////////// SETUP //////////////////////////////////////// */
void setup() {
//Motor Configuration
pinMode(pinStepX,OUTPUT);
pinMode(pinDirX, OUTPUT);
pinMode(pinStepY,OUTPUT);
pinMode(pinDirY, OUTPUT);
pinMode(pinEnableX,OUTPUT);
pinMode(pinEnableY,OUTPUT);

//Inputs Configuration
pinMode(pinHomeX,INPUT);
pinMode(pinLimX,INPUT);
pinMode(pinHomeY,INPUT);
pinMode(pinLimY,INPUT);
pinMode(pinEmergencyStop,INPUT);
pinMode(pinErrX,INPUT);
pinMode(pinErrY,INPUT);

//Start Outputs
StateMotor(MOTORX, ENABLE);
StateMotor(MOTORY, ENABLE);
DirMotor(MOTORX, RIGHT);
DirMotor(MOTORY, RIGHT);
digitalWrite(pinStepX,LOW); //Iniciar en nivel LOW
digitalWrite(pinStepY,LOW); //Iniciar en nivel LOW

//Serial Configuration
Serial.begin(115200);
inputString.reserve(200);
while(!Serial);
Serial.println("Motors Ready!");
HomeRequired = true;
}


/* //////////////////////////////////////// STATE MOTOR //////////////////////////////////////// */
void StateMotor(int motor, int state) {
if(motor == MOTORX)
    if(state == ENABLE)
        digitalWrite(pinEnableX,LOW);
    else
        digitalWrite(pinEnableX,HIGH);
else
    if(state == ENABLE)
        digitalWrite(pinEnableY,LOW);
    else
        digitalWrite(pinEnableY,HIGH);
}


/* //////////////////////////////////////// DIR MOTOR //////////////////////////////////////// */
void DirMotor(int motor, int dir) {
if(motor == MOTORX)
    if(dir == RIGHT)
        digitalWrite(pinDirX,HIGH);
    else
        digitalWrite(pinDirX,LOW);
else
    if(dir == RIGHT)
        digitalWrite(pinDirY,HIGH);
    else
        digitalWrite(pinDirY,LOW);
}


/* //////////////////////////////////////// HOME //////////////////////////////////////// */
void MotorHome() {
    unsigned long currTime;
    int tiempoFastMax=1000;
    int tiempoFastMin=300;
    int tiempoFastCurrent=tiempoFastMax;
    int tiempoSlow=5000;

    int SensorHomeX=0,SensorHomeY=0;
    int levelX=HIGH;
    int levelY=HIGH;

    currTime = micros();
    unsigned long tmrInitX = currTime;
    unsigned long tmrInitY = currTime;
    unsigned long tmrInitZ = currTime;
    DirMotor(MOTORX, LEFT);
    DirMotor(MOTORY, LEFT);
    while(true)
    {
        SensorHomeX=digitalRead(pinHomeX);
        SensorHomeY=digitalRead(pinHomeY);

        currTime = micros();

        if(SensorHomeX==0) {
            if(currTime-tmrInitX > tiempoFastCurrent) {              
                digitalWrite(pinStepX,levelX);
                (levelX==HIGH) ? levelX=LOW : levelX=HIGH;
                tmrInitX=currTime;
            }
        }

        if(SensorHomeY==0) {
            if(currTime-tmrInitY > tiempoFastCurrent) {              
                digitalWrite(pinStepY,levelY);
                (levelY==HIGH) ? levelY=LOW : levelY=HIGH;
                tmrInitY=currTime;
            }
        }

        if(currTime-tmrInitZ > 100) {
            tiempoFastCurrent--;
            if(tiempoFastCurrent<tiempoFastMin) tiempoFastCurrent=tiempoFastMin;
            tmrInitZ=currTime;
        }
        
        if(SensorHomeX==1 && SensorHomeY==1) break;
    }

    //Disminuye el golpe al llegar a home
    StateMotor(MOTORX, DISABLE);
    StateMotor(MOTORY, DISABLE);
    delay(200);
    StateMotor(MOTORX, ENABLE);
    StateMotor(MOTORY, ENABLE);

    currTime = micros();
    tmrInitX = currTime;
    tmrInitY = currTime;
    DirMotor(MOTORX, RIGHT);
    DirMotor(MOTORY, RIGHT);
    while(true)
    {
        SensorHomeX=digitalRead(pinHomeX);
        SensorHomeY=digitalRead(pinHomeY);

        currTime = micros();

        if(SensorHomeX==1)
            if(currTime-tmrInitX > tiempoSlow) {              
                digitalWrite(pinStepX,levelX);
                (levelX==HIGH) ? levelX=LOW : levelX=HIGH;
                tmrInitX=currTime;
            }
        if(SensorHomeY==1)
            if(currTime-tmrInitY > tiempoSlow) {              
                digitalWrite(pinStepY,levelY);
                (levelY==HIGH) ? levelY=LOW : levelY=HIGH;
                tmrInitY=currTime;
            }

        if(SensorHomeX==0 && SensorHomeY==0) break;
    }
}


/* //////////////////////////////////////// DISTANCIA //////////////////////////////////////// */
void Distancia() {
    int SensorLimX=0,SensorLimY=0;
    long distanciaX=0,distanciaY=0;

    int tiempo=300;

    unsigned long tmrInitX = micros();
    unsigned long tmrInitY = micros();
    int levelX=HIGH;
    int levelY=HIGH;

    DirMotor(MOTORX, RIGHT);
    DirMotor(MOTORY, RIGHT);
    while(true)
    {
        SensorLimX=digitalRead(pinLimX);
        SensorLimY=digitalRead(pinLimY);

        if(SensorLimX==0) {
            if(micros()-tmrInitX > tiempo) {              
                digitalWrite(pinStepX,levelX);
                if(levelX==LOW) distanciaX++;
                (levelX==HIGH) ? levelX=LOW : levelX=HIGH;
                tmrInitX=micros();
            }
        }

        if(SensorLimY==0) {
            if(micros()-tmrInitY > tiempo) {              
                digitalWrite(pinStepY,levelY);
                if(levelY==LOW) distanciaY++;
                (levelY==HIGH) ? levelY=LOW : levelY=HIGH;
                tmrInitY=micros();
            }
        }

        if(SensorLimX==1 && SensorLimY==1) break;
    }
    Serial.print("DistX:");
    Serial.print(distanciaX/2);
    Serial.print(" DistY:");
    Serial.println(distanciaY/2);
}


/* //////////////////////////////////////// MOVER //////////////////////////////////////// */
bool MoveAdvanceXY(long pulsosX, long pulsosY) {
    unsigned long currTime;         //Guarda el tiempo actual en cada iteracion
    unsigned long tiempoX,tiempoY;  //Variables para medir el intervalo del tiempo
    long recorridoX,recorridoY;     //Cantidad positiva de pulsos de movimiento

    //Determinar direccion del motor en base al signo
    if(pulsosX>0) DirMotor(MOTORX, RIGHT);
    else DirMotor(MOTORX, LEFT);
    recorridoX=abs(pulsosX);

    if(pulsosY>0) DirMotor(MOTORY, RIGHT);
    else DirMotor(MOTORY, LEFT);
    recorridoY=abs(pulsosY);

    //Cargar parametros de velocidad X
    int X_INTERVALO_MIN = 1;      //Intervalo de incremento Inicial (uS)
    int X_INTERVALO_MAX = 15000;  //Intervalo de incremento Final (uS)
    int X_VEL_INICIAL   = 440;    //Velocidad Inicial (uS)
    int X_VEL_FINAL     = 23;     //Velocidad Final (uS)

    //Cargar parametros de velocidad Y
    int Y_INTERVALO_MIN = 1;      //Intervalo de incremento Inicial (uS)
    int Y_INTERVALO_MAX = 15000;  //Intervalo de incremento Final (uS)
    int Y_VEL_INICIAL   = 450;    //Velocidad Inicial (uS)
    int Y_VEL_FINAL     = 26;     //Velocidad Final (uS)

    //Variables de funcionamiento X
    int VelocidadX = X_VEL_INICIAL;
    int FuncionX   = 0;
    int distX      = 0;
    int IntervaloX = X_INTERVALO_MIN;
    int CambioX    = (X_INTERVALO_MAX-X_INTERVALO_MIN)/2;
    tiempoX        = micros();

    //Variables de funcionamiento Y
    int VelocidadY = Y_VEL_INICIAL;
    int FuncionY   = 0;
    int distY      = 0;
    int IntervaloY = Y_INTERVALO_MIN;
    int CambioY    = (Y_INTERVALO_MAX-Y_INTERVALO_MIN)/2;
    tiempoY        = micros();

    unsigned long cronometroX = micros();
    unsigned long cronometroY = micros();
    int levelX=HIGH;
    int levelY=HIGH;
    
    //Algoritmo para mover motores
    int pasoX=0;
    int pasoY=0;
    while(true)
        {
        if(pasoX>=recorridoX && pasoY>=recorridoY) break;
        currTime = micros();
        
        if(pasoX<recorridoX)
                {
                if(currTime>=cronometroX+VelocidadX && levelX==HIGH) {
                    levelX=LOW;
                    digitalWrite(pinStepX,HIGH);
                    cronometroX=currTime;
                }
                else if(currTime>=cronometroX+VelocidadX && levelX==LOW) {
                    levelX=HIGH;
                    digitalWrite(pinStepX,LOW);
                    if(CheckLimitsX()!="OK") return false;
                    cronometroX=currTime;
                    pasoX++;

                    if(pasoX<CambioX && pasoX<recorridoX/2) IntervaloX+=5;
                    else if(pasoX>recorridoX-CambioX && pasoX>recorridoX/2) IntervaloX-=5;

                    if(currTime-tiempoX>IntervaloX)
                        {
                        tiempoX=currTime;
                        if(FuncionX==0)
                            {
                            VelocidadX-=2;
                            if(VelocidadX<X_VEL_FINAL || pasoX>recorridoX/2) 
                                {
                                FuncionX=1;
                                distX=pasoX;  //Cantidad requerida de pasos para acelerar/desacelerar
                                }
                            }
                        if(FuncionX==1)
                            if(pasoX>=recorridoX-distX) FuncionX=2;
                        if(FuncionX==2)
                            VelocidadX+=2;
                        }
                    }
                }

        if(pasoY<recorridoY)
                {
                if(currTime>=cronometroY+VelocidadY && levelY==HIGH) {
                    levelY=LOW;
                    digitalWrite(pinStepY,HIGH);
                    cronometroY=currTime;
                }
                else if(currTime>=cronometroY+VelocidadY && levelY==LOW) {
                    levelY=HIGH;
                    digitalWrite(pinStepY,LOW);
                    if(CheckLimitsY()!="OK") break;
                    cronometroY=currTime;
                    pasoY++;

                    if(pasoY<CambioY && pasoY<recorridoY/2) IntervaloY+=5;
                    else if(pasoY>recorridoY-CambioY && pasoY>recorridoY/2) IntervaloY-=5;

                    if(currTime-tiempoY>IntervaloY)
                        {
                        tiempoY=currTime;
                        if(FuncionY==0)
                            {
                            VelocidadY-=2;
                            if(VelocidadY<Y_VEL_FINAL || pasoY>recorridoY/2) 
                                {
                                FuncionY=1;
                                distY=pasoY;  //Cantidad requerida de pasos para acelerar/desacelerar
                                }
                            }
                        if(FuncionY==1)
                            if(pasoY>=recorridoY-distY) FuncionY=2;
                        if(FuncionY==2)
                            VelocidadY+=2;
                        }
                    }
                }
        }
    return true;
}


/* //////////////////////////////////////// CHECK INPUTS //////////////////////////////////////// */
String CheckFlags() {
    EmergencyStop = digitalRead(pinEmergencyStop);  //0:Normal    1:Presionado
    ErrX = !digitalRead(pinErrX);                   //0:OK        1:Error
    ErrY = !digitalRead(pinErrY);                   //0:OK        1:Error

    if(EmergencyStop == true)
        return "EmergencyStop Activated";
    else if(ErrX == true)
        return "Error on Driver X";
    else if(ErrY == true)
        return "Error on Driver Y";
    return "OK";
}

String CheckLimitsX() {
    int SensorHomeX=digitalRead(pinHomeX);
    int SensorLimX=digitalRead(pinLimX);
    
    if(SensorHomeX == true)
        return "Alert Limit on HomeX";
    else if(SensorLimX == true)
        return "Alert Limit on LimX";
    return "OK";
}

String CheckLimitsY() {
    int SensorHomeY=digitalRead(pinHomeY);
    int SensorLimY=digitalRead(pinLimY);
    
    if(SensorHomeY == true)
        return "Alert Limit on HomeY";
    else if(SensorLimY == true)
        return "Alert Limit on LimY";
    return "OK";
}


/* //////////////////////////////////////// LOOP //////////////////////////////////////// */
void loop() {
    CheckFlags();
    
    if(stringComplete)
    {
        if(inputString=="RESET") {
            Serial.println("Reset uC");
            delay(50);
            resetFunc();
        }
        else if(EmergencyStop == true) {
            HomeRequired = true;
            Serial.println("EmergencyStop Activated");
            Serial.println("ERROR");
        }
        else if(ErrX == true) {
            HomeRequired = true;
            Serial.println("Error on Driver X");
            Serial.println("ERROR");
        }
        else if(ErrY == true) {
            HomeRequired = true;
            Serial.println("Error on Driver Y");
            Serial.println("ERROR");
        }
        else if(inputString=="STOP") {
            DetenerButton = true;
            Serial.println("Sistema detenido por comando");
            Serial.println("OK");
        }
        else if(inputString=="INIT") {
            DetenerButton = false;
            Serial.println("Sistema iniciado por comando");
            Serial.println("OK");
        }
        else if(DetenerButton == true) {
            Serial.println("Sistema detenido por comando");
            Serial.println("ERROR");
        }
        else if(inputString=="HOME") {
            Serial.println("Finding Home...");
            MotorHome();
            HomeRequired = false;
            Serial.println("OK");
        }
        else if(inputString=="DIST") {
            Serial.println("Measuring Distance...");
            Distancia();
            Serial.println("OK");
        }
        else if(inputString=="READ") {
            Serial.print("HomeX:");
            Serial.print(digitalRead(pinHomeX));
            Serial.print(" LimX:");
            Serial.print(digitalRead(pinLimX));
            Serial.print(" HomeY:");
            Serial.print(digitalRead(pinHomeY));
            Serial.print(" LimY:");
            Serial.print(digitalRead(pinLimY));
            Serial.println("");
            Serial.print("Stop:");
            Serial.print(EmergencyStop);
            Serial.print(" ErrX:");
            Serial.print(ErrX);
            Serial.print(" ErrY:");
            Serial.print(ErrY);
            Serial.println("");
            Serial.println("OK");
        }
        else if(inputString=="ENX") {
            StateMotor(MOTORX, ENABLE);
            Serial.println("Motor X Enabled");
            Serial.println("OK");
        }
        else if(inputString=="DISX") {
            StateMotor(MOTORX, DISABLE);
            Serial.println("Motor X Disabled");
            Serial.println("OK");
        }
        else if(inputString=="ENY") {
            StateMotor(MOTORY, ENABLE);
            Serial.println("Motor Y Enabled");
            Serial.println("OK");
        }
        else if(inputString=="DISY") {
            StateMotor(MOTORY, DISABLE);
            Serial.println("Motor Y Disabled");
            Serial.println("OK");
        }
        else if(HomeRequired == true) {
            Serial.println("ERROR Home Requerido");
        }
        else {
            int tam=inputString.length()+1;
            char arr[tam];
            inputString.toCharArray(arr, tam);

            char *name = strtok(arr, ",");
            String comand(name);
            name = strtok(NULL, ",");
            int x = atoi(name);
            name = strtok(NULL, ",");
            int y = atoi(name);

            if(comand=="M") {
                bool result0 = MoveAdvanceXY(2*x,2*y);
                String result1 = CheckFlags();
                String result2 = CheckLimitsX();
                String result3 = CheckLimitsY();
                if(result0==true && result1=="OK" && result2=="OK" && result3=="OK") {
                    Serial.print("X:");
                    Serial.print(x);
                    Serial.print(" Y:");
                    Serial.println(y);
                    Serial.println("OK");
                }
                else {
                    HomeRequired = true;
                    Serial.print("Flags: ");
                    Serial.println(result1);
                    Serial.print("LimitsX: ");
                    Serial.println(result2);
                    Serial.print("LimitsY: ");
                    Serial.println(result3);
                    Serial.println("ERROR");
                }
            }
            else
              HomeRequired = true;
        }

        inputString = "";
        stringComplete = false;
    }
}


/* //////////////////////////////////////// SERIAL EVENT //////////////////////////////////////// */
void serialEvent() {
    while(Serial.available()) {
        char inChar = (char)Serial.read();  //Get the new byte:
        if(inChar=='\n')
            stringComplete = true; //If the incoming character is a newline, set a flag so the main loop
        else
            inputString += inChar; //Add it to the inputString
    }
}
