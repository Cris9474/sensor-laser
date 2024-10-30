#include <windows.h> // Para la interface
#include <string.h>  // Para el comparador de cadenas
#include <stdio.h>   // Salidas de texto en la terminar
#include <stdint.h>  // Para los tipos de datos enteros
#include <stdlib.h>  // Para exit
#include <unistd.h>  // Para el Sleep
#include <time.h>

#define ID_LABEL 100
#define ID_BTNCDF 101
#define ID_BTNCDT 102
#define ID_BTNCONTINUAR 103
#define ID_TEMP 105
#define ID_MUESTRA 106
#define ID_FREC 107
#define ID_TIEMP 108
#define ID_COMBOBOX1 109
#define ID_COMBOBOX2 110
//PARA SABER CUAL OPCION DEL COMBOBOX SE ELIGIO
char sensorE[10];
char senalE[10];
int indiceSenal = 0;
float lec();
float arranque();
float diftemp();
LRESULT CALLBACK ProcediementoVentana(HWND hwnd,UINT msg, WPARAM wParam,LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstanciaActual,HINSTANCE hInstanciaPrevia,LPSTR lpCmdLinea,int nCmdShow){
    HWND ventana; //manejador de ventana
    MSG mensaje; //intermediario para comunicar los mensajes de la aplicacion    WNDCLASSEX clase;
    WNDCLASSEX clase; //estructura que contienen un conjunto de elementos que caracterizan a una ventana
    clase.cbSize = sizeof(WNDCLASSEX); //tamaño de la clase es igual al tamaño de la estructura    clase.cbClsExtra = 0;
    clase.cbClsExtra = 0; //El número de bytes adicionales para asignar (El sistema inicializa los bytes a cero.)
    clase.style = CS_HREDRAW | CS_VREDRAW;  //se puede dibujar horizontalmente y verticalmente
    clase.lpfnWndProc = ProcediementoVentana;
    /* lpfnWndProc:
    Es la responsable de todo lo relativo al aspecto y al comportamiento de una ventana.
    Puntero a una función de este tipo, esa función es la que se encargará de procesar
    todos los mensajes para esa clase de ventana. Cuando registremos nuestra clase de
    ventana, tendremos que asignar a ese miembro el puntero a nuestro procedimiento de ventana.*/
    clase.hInstance = hInstanciaActual;
    clase.hIcon = LoadIcon(NULL,0);//Identificador de icono
    clase.hIconSm =LoadIcon(NULL,IDI_APPLICATION);//Elegimos que icono queremos
    clase.hCursor = LoadCursor(NULL,IDC_ARROW);//Identificador de cursor, elegimos la forma en que se mostrará
    clase.lpszClassName = "IdentificadorVentana"; //este es el identificador para pasarlo a CreateWindowsEx();
    clase.hbrBackground =CreateSolidBrush(RGB(127,1,32)); //color de la ventana

    if(!RegisterClassEx(&clase)){ //si no esta registrada la clase
        MessageBox( NULL,"No se pudo ejecutar la aplicacion","Error",MB_ICONERROR);
        return EXIT_FAILURE;
    }
    //WS_OVERLAPPED|WS_MINIMIZEBOX|WS_SYSMENU (para que no se maximice)
    ventana = CreateWindowEx(0,"IdentificadorVentana","PANTALLA PRINCIPAL",WS_OVERLAPPEDWINDOW |WS_SYSMENU,
                             400,80,560,630,HWND_DESKTOP,NULL,hInstanciaActual,NULL);
    //ENCABEZADO //x,y,largo,ancho
    CreateWindowW(L"Static",L" Centro de Investigacion de Ciencia Aplicada y Tecnología Avanzada", ES_CENTER|WS_VISIBLE|WS_CHILD|SS_NOTIFY,150,60,250,40,ventana,(HMENU)ID_LABEL,hInstanciaActual,NULL);                                                                                //x,y,largo,ancho
    CreateWindowW(L"Static",L" Unidad Legaria", WS_VISIBLE|WS_CHILD|SS_NOTIFY,220,100,110,20,ventana,(HMENU)ID_LABEL,hInstanciaActual,NULL);
    CreateWindowW(L"Static",L" Temperatura inicial:", WS_VISIBLE|WS_CHILD|SS_NOTIFY,180,160,135,20,ventana,(HMENU)ID_LABEL,hInstanciaActual,NULL);
    CreateWindowW(L"Static",L"°C", WS_VISIBLE|WS_CHILD|SS_NOTIFY,355,160,20,20,ventana,(HMENU)ID_LABEL,hInstanciaActual,NULL);
    CreateWindowW(L"Static",L"_____________________________________________________________________", WS_VISIBLE|WS_CHILD|SS_NOTIFY,0,200,560,20,ventana,(HMENU)ID_LABEL,hInstanciaActual,NULL);
    //TAL VEZ HAY QUE BORRAR BUSCAR COMO APARECER TEXTO
    //CreateWindowEx(0,"EDIT","",ES_NUMBER|ES_AUTOHSCROLL|ES_CENTER|WS_CHILD|WS_VISIBLE,250,170,40,20,ventana,(HMENU)ID_TEMP,hInstanciaActual,NULL);
    CreateWindowW(L"Static",L" Nombre de la muestra: ", WS_VISIBLE|WS_CHILD|SS_NOTIFY,80,260,160,20,ventana,(HMENU)ID_LABEL,hInstanciaActual,NULL);
    CreateWindowEx(0,"EDIT","",ES_NUMBER|ES_AUTOHSCROLL|ES_CENTER|WS_CHILD|WS_VISIBLE,250,260,40,20,ventana,(HMENU)ID_MUESTRA,hInstanciaActual,NULL);
    CreateWindowW(L"Static",L" Selecciona el sensor: ", WS_VISIBLE|WS_CHILD|SS_NOTIFY,80,340,150,20,ventana,(HMENU)ID_LABEL,hInstanciaActual,NULL);
    CreateWindowW(L"Static",L" Tiempo de medición [s]: ", WS_VISIBLE|WS_CHILD|SS_NOTIFY,80,300,180,30,ventana,(HMENU)ID_TIEMP,hInstanciaActual,NULL);
    CreateWindowEx(0,"EDIT","",ES_NUMBER|ES_AUTOHSCROLL|ES_CENTER|WS_CHILD|WS_VISIBLE,250,300,40,20,ventana,NULL,hInstanciaActual,NULL);
    CreateWindowW(L"Static",L" Frecuencia [Hz]:", WS_VISIBLE|WS_CHILD|SS_NOTIFY,80,420,120,20,ventana,(HMENU)ID_LABEL,hInstanciaActual,NULL);
    CreateWindowW(L"Static",L" Tipo de señal: ", WS_VISIBLE|WS_CHILD|SS_NOTIFY,80,380,120,20,ventana,(HMENU)ID_LABEL,hInstanciaActual,NULL);
    CreateWindowEx(0,"BUTTON","Continuar",BS_CENTER|WS_CHILD|WS_VISIBLE,200,500,150,30,ventana,(HMENU)ID_BTNCONTINUAR,NULL,NULL);

    ShowWindow(ventana,nCmdShow); //mostramos la ventana
    UpdateWindow(ventana);//actualizamos la ventana en caso hagamos algunos eventos
    //creamos el bucle para saber si nuestro mensaje esta interactuando con el usuario, si será cero significa que acabo el programa
    while(GetMessage(&mensaje,NULL,0,0)>0){
            TranslateMessage(&mensaje);
            DispatchMessage(&mensaje);
    }

}

// Función para leer la temperatura del sensor a través del puerto serial
float lec() {
    HANDLE hSerial;
    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts = {0};
    DWORD bytesRead;
    char buffer[2];  // Para almacenar los 2 bytes de la respuesta

    // Abrir el puerto COM (cambia "COM5" por el puerto que estás utilizando)
    hSerial = CreateFile("COM5", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hSerial == INVALID_HANDLE_VALUE) {
        printf("Error al abrir el puerto serial\n");
        exit(1);
    }

    // Configurar parámetros del puerto serial
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        printf("Error al obtener el estado del puerto serial\n");
        CloseHandle(hSerial);
        exit(1);
    }

    dcbSerialParams.BaudRate = CBR_9600;  // Velocidad a 9600 baudios
    dcbSerialParams.ByteSize = 8;         // 8 bits de datos
    dcbSerialParams.StopBits = ONESTOPBIT; // 1 bit de parada
    dcbSerialParams.Parity = NOPARITY;    // Sin paridad

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        printf("Error al configurar el puerto serial\n");
        CloseHandle(hSerial);
        exit(1);
    }

    // Configuración de tiempos de espera
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts)) {
        printf("Error al configurar los tiempos de espera del puerto serial\n");
        CloseHandle(hSerial);
        exit(1);
    }

    // Limpiar buffers y enviar comando
    PurgeComm(hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);
    Sleep(800);  // Pausa de 0.8 segundos

    // Enviar el comando 0x01 al sensor
    char command = 0x01;
    DWORD bytesWritten;
    WriteFile(hSerial, &command, 1, &bytesWritten, NULL);

    // Esperar 0.01 segundos antes de leer la respuesta
    Sleep(10);

    // Leer 2 bytes de la respuesta del sensor
    ReadFile(hSerial, buffer, 2, &bytesRead, NULL);

    if (bytesRead < 2) {
        printf("Error al leer datos del sensor\n");
        CloseHandle(hSerial);
        exit(1);
    }

    // Convertir los bytes recibidos en temperatura
    int16_t temp1 = (int8_t)buffer[0];  // Byte 1
    int16_t temp2 = (int8_t)buffer[1];  // Byte 2

    float temsen = ((temp1 * 255.987) + temp2 - 10000) / 100.0;

    // Cerrar el puerto serial
    CloseHandle(hSerial);

    return temsen;
}

float arranque() {
    float numtemp;
    numtemp = lec();  // Llama a la función `lec` para obtener la temperatura
    //printf("La temperatura inicial es: %.2f°C\n", numtemp);
    return numtemp;  // Devuelve la temperatura inicial
}

float diftemp() {
    float numactual;
    numactual = lec();  // Llama a la función `lec` para obtener la temperatura actual
    printf("La temperatura actual es: %.2f°C\n", numactual);
    return numactual;
}

/*LRESULT : utiliza para representar el valor
 de retorno de los procedimientos de ventana.
 Suele ser un LARGO (de ahí la L ).
 Parámetros del procedimiento de ventana :
        hwnd es el manipulador de la ventana a la que está destinado el mensaje.
        msg es el código del mensaje.
        wParam es el parámetro de tipo palabra asociado al mensaje.
        lParam es el parámetro de tipo doble palabra asociado al mensaje.
 */
LRESULT CALLBACK ProcediementoVentana(HWND hwnd,UINT msg, WPARAM wParam,LPARAM lParam){
    
    //Declaracion de variables para las imagenes de cabecera
    HWND himg1;
    HWND himg2;
    static HWND hEdit;
    HBITMAP cargar_img1;
    HBITMAP cargar_img2;
    cargar_img1= (HBITMAP)LoadImageW(NULL,L"cicata.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
    cargar_img2= (HBITMAP)LoadImageW(NULL,L"ipn.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
    //Declaracion de variables para el COMBOBOX
    int nitems;
    const char * sensores[] = {"Frontal","Trasero"};
    nitems = (sizeof(sensores)/(sizeof(char*)));

    int  senales;
    const char * senal[] = {"Continua","Modulada"};
    senales = (sizeof(senal)/(sizeof(char*)));
    //static HINSTANCE Instancia; //no se destruye por ser estatica
    switch(msg){
                case WM_CREATE:{
                    float ini = arranque();
                    CHAR temp[10];
                    sprintf(temp, "%.2f", ini);
                    CreateWindow("STATIC",temp,ES_CENTER|WS_CHILD|WS_VISIBLE,315,160,40,20,hwnd,NULL,NULL,NULL);
                    CreateWindow("COMBOBOX","",CBS_DROPDOWNLIST|WS_CHILD|WS_VISIBLE,250,340,150,200,hwnd,(HMENU)ID_COMBOBOX1,NULL,NULL);
                    CreateWindow("COMBOBOX","",CBS_DROPDOWNLIST|WS_CHILD|WS_VISIBLE,250,380,150,200,hwnd,(HMENU)ID_COMBOBOX2,NULL,NULL);
                    hEdit = CreateWindowEx(0,"EDIT","",ES_NUMBER|ES_AUTOHSCROLL|ES_CENTER|WS_CHILD|WS_VISIBLE|WS_BORDER,250,420,50,20,hwnd,(HMENU)ID_FREC,NULL,NULL);
                    for(int i=0; i<nitems; i++){
                        SendDlgItemMessage(hwnd,ID_COMBOBOX1,CB_ADDSTRING,0,(LPARAM)sensores[i]);
                    }
                    for(int j=0; j<senales; j++){
                        SendDlgItemMessage(hwnd,ID_COMBOBOX2,CB_ADDSTRING,0,(LPARAM)senal[j]);
                    }
                    
                    //Instancia =((LPCREATESTRUCT)lParam)->hInstance; //cassting para convertir en una structura que lleva almacenada la info de la ventana.
                    //CreateWindowW(L"Static",L" Unidad Legaria", WS_VISIBLE|WS_CHILD|SS_NOTIFY,230,90,150,20,ventana,(HMENU)ID_LABEL,hInstanciaActual,NULL);                                                                                //x,y,largo,ancho

                    himg1 = CreateWindowW(L"Static",NULL,WS_VISIBLE | WS_CHILD | SS_BITMAP, 430,50,0,0,hwnd,NULL,NULL,NULL);
                    himg2 = CreateWindowW(L"Static",NULL,WS_VISIBLE | WS_CHILD | SS_BITMAP, 70,45,0,0,hwnd,NULL,NULL,NULL);

                    SendMessageW(himg1,STM_SETIMAGE,IMAGE_BITMAP,(WPARAM)cargar_img1);
                    SendMessageW(himg2,STM_SETIMAGE,IMAGE_BITMAP,(WPARAM)cargar_img2);
                }
                case WM_COMMAND:{
                    switch (LOWORD(wParam)){
                        case ID_FREC:{//PASA CUANDO TOCAS ALGUN ID LLAMADO ID_LABEL
                        indiceSenal = SendDlgItemMessage(hwnd, ID_COMBOBOX2,CB_GETCURSEL,0,0);//indica donde seleccione el combobox
                        SendDlgItemMessage(hwnd,ID_COMBOBOX2,CB_GETLBTEXT,indiceSenal,(LPARAM)senalE);
                        if(strcmp(senalE,"Continua")==0){
                            EnableWindow(hEdit, FALSE);
                        }else{
                            EnableWindow(hEdit, TRUE);
                        }
                        break;    
                        }
                        case ID_BTNCONTINUAR:{
                            //MessageBox(hwnd,"Entramos","Salir.",MB_OKCANCEL|MB_ICONQUESTION);
                            //Para la lectura del archivo con los datos del sensor
                            FILE *fpPuntos = fopen("puntos.txt","w+");
                            if (fpPuntos == NULL) {
                                MessageBox(hwnd, "No se pudo abrir puntos.txt", "Error", MB_ICONERROR);
                                return -1;
                            }
                            FILE *fp = popen("gnuplot -persist","w");
                            if (fp == NULL) {
                                MessageBox(hwnd, "No se pudo abrir gnuplot", "Error", MB_ICONERROR);
                                fclose(fpPuntos);  // Cerrar el archivo si hay error en gnuplot
                                return -1;
                            }
                            fprintf(fp, "set title 'Gráfica en Tiempo Real'\n");
                            fprintf(fp, "set xlabel 'Tiempo (s)'\n");
                            fprintf(fp, "set ylabel 'Temperatura'\n");
                            fprintf(fp, "set grid\n");
                            fprintf(fp, "set yrange [0:100]\n"); // Rango en el eje Y para que sea claro en la visualización
                            fprintf(fp, "plot '-' with lines title 'Sensor Data'\n");
                            
                            //int indiceItem1 = SendDlgItemMessage(hwnd,ID_COMBOBOX1,CB_GETCURSEL,0,0);
                            //SendDlgItemMessage(hwnd,ID_COMBOBOX1,CB_GETLBTEXT,indiceItem1,(LPARAM)sensorE);
                            //int indiceItem2 = SendDlgItemMessage(hwnd,ID_COMBOBOX2,CB_GETCURSEL,0,0);
                            //SendDlgItemMessage(hwnd,ID_COMBOBOX1,CB_GETLBTEXT,indiceItem2,(LPARAM)sensorE);
                            char noMuestra[10];//Esta variable es para guardar el resultado del edit de no.Muestra como cadena
                            char tMedicion[10];
                            char frec[10];
                            GetDlgItemText(hwnd,ID_MUESTRA,noMuestra,10);//Obtiene el valor que es ingresado como string
                            GetDlgItemText(hwnd,ID_TIEMP,tMedicion,10);
                            GetDlgItemText(hwnd,ID_FREC,frec,10);
                            double num1 =atof(noMuestra);//Convierte el string a un double para poder trabajar con el
                            double num2 =atof(tMedicion);
                            double num3 =atof(frec);
                            int indiceSensor = SendDlgItemMessage(hwnd, ID_COMBOBOX1,CB_GETCURSEL,0,0);//indica donde seleccione el combobox
                            SendDlgItemMessage(hwnd,ID_COMBOBOX1,CB_GETLBTEXT,indiceSensor,(LPARAM)sensorE);
                            if(strcmp(sensorE,"Frontal")==0){
                                if(strcmp(senalE,"Continua")==0){
                                    printf("Aqui ando3");
                                    fprintf(fpPuntos,"1.0 2.0\n");
                                    fprintf(fpPuntos,"2.0 3.0\n");
                                    fprintf(fpPuntos,"3.0 4.0\n");
                                    fclose(fpPuntos);
                                    FILE *fp = popen("gnuplot -persist","w");
                                    if (fp == NULL) {
                                        MessageBox(hwnd, "No se pudo abrir gnuplot", "Error", MB_ICONERROR);
                                        fclose(fpPuntos);  // Cerrar el archivo si hay error en gnuplot
                                        return -1;
                                    }
                                    fprintf(fp,"plot \"puntos.txt\" with lines\n");
                                    fclose(fp);
                                }else{

                                }

                            }else if(strcmp(sensorE,"Trasero")==0){
                                if(strcmp(senalE,"Continua")==0){
                                    
                                }else{

                                }    
                            }else{//ESTE ES PARA HABER LAS GRAFICAS AL MISMO TIEMPO (SE PUEDE BORRAR)
                                if(strcmp(senalE,"Continua")==0){

                                }else{

                                }    
                            }
                            break;
                        }
                    }
                break;
                }
                case WM_DESTROY:{
                    PostQuitMessage(0); //este cero pasará al bucle while para que acabe el programa
                break;
                }
                case WM_CTLCOLORSTATIC:{
                    HDC hdcStatic = (HDC)wParam;
                    SetTextColor(hdcStatic,RGB(255,255,255));
                    SetBkColor(hdcStatic,RGB(127,1,32));
                    return (INT_PTR)CreateSolidBrush(RGB(127,1,32));
                }

                default:{
                return DefWindowProc(hwnd, msg,wParam,lParam);
                }

    }
    return 0;
}