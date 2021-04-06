
struct field
{
	struct field *patual;
	char fieldname[50];
	char type;
	int width;
	int dec;
	struct type *pdados;
	struct field *prox;
};

union typeinfo
{
	float number;
	char date[10];
	char logical;
	char character[50];
	char memo[50];	
};

struct type
{
	char terminal;
	union typeinfo no;
	struct type *prox;
};

typedef struct type Type;



typedef struct field Field;

struct typelogical
{
	char Status;
	struct typelogical *prox;
};

struct database
{
	struct database *ant;
	char nomedbf[50];
	char data[10];
	char hora[5];
	struct typelogical *status;
	struct database *prox;
	struct field *campos;
};
typedef struct database Database;

struct unidade
{
	struct unidade *top;
	struct unidade *bottom;
	struct database *arqs;
	char und[3];
};
typedef struct unidade Unidade;
void limpatela();
void desenhaBorda();
void limpaParcial();
void telainicial();

void createDir(Unidade **und,char text[3])
{
	if((*und) == NULL)
	{
		*und = (Unidade*)malloc(sizeof(Unidade));
		strcpy((*und)->und,text);
		(*und)->bottom = NULL;
		(*und)->top = NULL;
		(*und)->arqs = NULL;
	}
	else
	{
		Unidade *aux = *und;
		Unidade *nova = (Unidade*)malloc(sizeof(Unidade));
		strcpy(nova->und,text);
		while(aux->top != NULL)
		{
			aux = aux->top;
		}
		aux->top = nova;
		nova->bottom = aux->top;
		nova->top = NULL;
		nova->arqs = NULL;
	}
}

Unidade *buscaDir(Unidade *und,char value[3])
{
	Unidade *posicao;
	if(und == NULL)
	{
		posicao = NULL;
	}
	else
	{
		while(und != NULL && strcmp(value,und->und) != 0)
		{
			und = und->top;
		}
		posicao = und;
		if(posicao != NULL)
		{
			gotoxy(80,27);
			printf("Letter Disk Found %s\n",posicao->und);
			gotoxy(80,28);
			printf("Press Any Key to Continue");
		}
		else
		{
			gotoxy(80,27);
			printf("Letter Disk not Found %s\n",value);
			gotoxy(80,28);
			printf("Press Any Key to Continue");
		}
		getch();
	}
	return posicao;
}

char create(char text[100])
{
	return (text[0] == 'C' && text[1] == 'R' && text[2] == 'E' && text[3] == 'A' && text[4] == 'T' && text[5] == 'E');	
}

char *getTipo(char tipo)
{
	char *retorno = NULL;
	retorno = (char*)malloc(sizeof(char));
	if(tipo == 'N')
		retorno = "Numeric";
	else if(tipo == 'D')
		retorno = "Date";
	else if(tipo == 'L')
		retorno = "Logical";
	else if(tipo == 'C')
		retorno = "Character";
	else if(tipo == 'M')
		retorno = "Memo";
	return retorno; 
}

char *hora(char *horario)
{
	struct tm *hora_atual;
	time_t segundos;
	time(&segundos);
	hora_atual = localtime(&segundos);
	char minutos[5],horas[5];
	itoa(hora_atual->tm_min,minutos,10);
	itoa(hora_atual->tm_hour,horas,10);
	strcpy(horario,horas);
	strcat(horario,":");
	strcat(horario,minutos);
	return horario;
}

char *data(char *date)
{
	struct tm *data;
	time_t segundos;
	time(&segundos);
	data = localtime(&segundos);
	char dia[5],mes[5],ano[5];
	itoa(data->tm_mday,dia,10);
	itoa(data->tm_mon+1,mes,10);
	itoa(data->tm_year+1900,ano,10);
	strcpy(date,dia);
	strcat(date,"/");
	strcat(date,mes);
	strcat(date,"/");
	strcat(date,ano);
	return date;
}

Database *buscarDatabase(Unidade **posicao,char database[20])
{
	Database *aux = (*posicao)->arqs;
	Database *pos = NULL;
	if(aux != NULL)
	{
		while(aux->prox != NULL && strcmp(database,aux->nomedbf) != 0)
			aux = aux->prox;
		pos = aux;
	}
	return pos;
}

int getRecords(Database *db)
{
	int i = 0;
	Database *aux = db;
	while(aux->campos != NULL)
	{
		while(aux->campos->pdados != NULL)
		{
			i++;
			db->campos->pdados = db->campos->pdados->prox;
		}
		aux->campos = aux->campos->prox;
	}
	return i;
}

int getSize(Database *db)
{
	int i=0;
	while(db->campos != NULL)
	{
		i += db->campos->width;
		db->campos = db->campos->prox;
	}
	return i;
}

void DesenhaDir()
{
	gotoxy(8,6);
	printf("Database Files");
	gotoxy(25,6);
	printf("# Records");
	gotoxy(38,6);
	printf("Last Update");
	gotoxy(55,6);
	printf("Size");
}


void Dir(Unidade *posicao)
{
	textcolor(BLACK);
	Unidade *aux = posicao;
	int i = 7;
	gotoxy(10,5);
	printf("Current Unit (%s)",posicao->und);
	DesenhaDir();
	while(aux->arqs != NULL)
	{
		gotoxy(8,i);
		printf("%s\n",aux->arqs->nomedbf);
		gotoxy(56,i);
		printf("%d",getSize(aux->arqs));
		gotoxy(40,i);
		printf("%s",aux->arqs->data);
		gotoxy(31,i);
		printf("%d",getRecords(aux->arqs));
		aux->arqs = aux->arqs->prox;
		i++;
	}
	gotoxy(8,i);
}

void criarArquivo(char *nome,Unidade **posicao)
{
	Database *arq = (Database*)malloc(sizeof(Database));
	Database *aux = (*posicao)->arqs;
	if((*posicao)->arqs == NULL)
	{
		(*posicao)->arqs = arq;
		arq->ant = NULL;
		char *date = (char*)malloc(sizeof(char));
		char *horario = (char*)malloc(sizeof(char));
		horario = hora(horario);
		date = data(date);
		strcpy(arq->hora,horario);
		strcpy(arq->data,date);
		strcpy(arq->nomedbf,nome);
		arq->status = NULL;
		arq->campos = NULL;
		arq->prox = NULL;
	}
	else
	{
		while(aux->prox != NULL)
		{
			aux = aux->prox;
		}
		aux->prox = arq;
		arq->ant = aux->prox;
		char *date = (char*)malloc(sizeof(char));
		char *horario = (char*)malloc(sizeof(char));
		horario = hora(horario);
		date = data(date);
		strcpy(arq->data,date);
		strcpy(arq->hora,horario);
		strcpy(arq->nomedbf,nome);
		arq->campos = NULL;
		arq->status = NULL;
		arq->prox = NULL;
	}
}

char set_default_to(char text[100])
{
	return (text[0] == 'S' && text[1] == 'E' && text[2] == 'T' && text[3] == ' ' && text[4] == 'D' && text[5] == 'E' && text[6] == 'F' && text[7] == 'A' && text[8] == 'U' && text[9] == 'L' && text[10] == 'T' && text[11] == ' ' && text[12] == 'T' && text[13] == 'O' && text[14] == ' ');
}

void desenhaCreate()
{
	int i;
	gotoxy(10,4);
	printf("Field Name");
	gotoxy(25,4);
	printf("Type");
	gotoxy(40,4);
	printf("Width");
	gotoxy(55,4);
	printf("Dec");
	for(i = 10;i <= 57;i++)
	{
		gotoxy(i,5);
		printf("%c",205);
	}
}

void typeFields(Unidade **posicao,char database[20])
{
	
	int i = 1;
	int pos = 6;
	char opcao = 1;
	int width,dec;
	char fieldname[20],type[20];
	while(opcao != 27 && i != 25)
	{
		Field *field = (Field*)malloc(sizeof(Field));
		Field *aux = NULL;
		Database *posi = buscarDatabase(*&posicao,database);
		gotoxy(6,pos);
		printf("%d",i);
		gotoxy(10,pos);
		fflush(stdin);
		gets(fieldname);
		gotoxy(25,pos);
		fflush(stdin);
		gets(type);
		gotoxy(40,pos);
		scanf("%d",&width);
		gotoxy(55,pos);
		scanf("%d",&dec);
		strcpy(field->fieldname,fieldname);
		if(strcmp(type,"Numeric") == 0)
			field->type = 'N';
		if(strcmp(type,"Date") == 0)
			field->type = 'D';
		if(strcmp(type,"Logical") == 0)
			field->type = 'L';
		if(strcmp(type,"Character") == 0)
			field->type = 'C';
		if(strcmp(type,"Memo") == 0)
			field->type = 'M';
		field->width = width;
		field->dec = dec;
		if(posi->campos == NULL)
		{
			posi->campos = field;
			field->prox = NULL;
			field->pdados = NULL;
		}
		else
		{
			aux = posi->campos;
			while(aux->prox != NULL)
				aux = aux->prox;
			aux->prox = field;
			field->prox = NULL;
			field->pdados = NULL;
		}
		i++;
		pos++;
		gotoxy(75,27);
		printf("ENTER to continue or ESC for return to main");
		opcao = getch();
		limpaParcial();
	}
	telainicial();
}

char Use(char cmd[30])
{
	return cmd[0] == 'U' && cmd[1] == 'S' && cmd[2] == 'E';
}

void ExibeRegistros(Unidade *posicao)
{
	gotoxy(10,5);
	printf("Record#");
	int records = 0,col = 25,lin,linr=6;
	Field *campos = posicao->arqs->campos;
	while(campos != NULL)
	{
		gotoxy(col,5);
		printf("%s",campos->fieldname);
		lin = 6;
		records = 0;
		linr=6;
		while(campos->pdados != NULL)
		{
			gotoxy(10,linr);
			linr++;
			records++;
			printf("%d",records);
			gotoxy(col,lin);
			if(campos->pdados->terminal == 'N')
				printf("%.2f",campos->pdados->no.number);
			else if(campos->pdados->terminal == 'C')
				printf("%s",campos->pdados->no.character);
			else if(campos->pdados->terminal == 'D')
				printf("%s",campos->pdados->no.date);
			else if(campos->pdados->terminal == 'L')
				printf("%c",campos->pdados->no.logical);
			else if(campos->pdados->terminal == 'M')
				printf("%s",campos->pdados->no.memo);
			campos->pdados = campos->pdados->prox;
			lin++;
		}
		campos = campos->prox;
		col+=15;
	}
}

void imprimeStructure(Unidade *posicao)
{
	int i = 1;
	int pos = 8;
	int somawidth = 0;
	Database *aux = posicao->arqs;
	gotoxy(10,4);
	printf("Structure for database: %s//%s",posicao->und,posicao->arqs->nomedbf);
	gotoxy(10,5);
	//printf("Number of data records: %d",getRecords(aux2));
	gotoxy(10,6);
	printf("Date of last update   : %s",posicao->arqs->data);
	gotoxy(10,7);
	printf("Field");
	gotoxy(18,7);
	printf("Field Name");
	gotoxy(30,7);
	printf("Type");
	gotoxy(42,7);
	printf("Width");
	gotoxy(50,7);
	printf("Dec");
	while(aux->campos != NULL)
	{
		gotoxy(10,pos);
		printf("%d",i);
		gotoxy(18,pos);
		printf("%s",aux->campos->fieldname);
		gotoxy(30,pos);
		printf("%s",getTipo(aux->campos->type));
		gotoxy(42,pos);
		printf("%d",aux->campos->width);
		somawidth += aux->campos->width;
		aux->campos = aux->campos->prox;
		i++;
		pos++;
	}
	gotoxy(10,pos);
	printf("** Total **");
	gotoxy(42,pos);
	printf("%d",somawidth);
}


void insereRegistro(Unidade **posicao)
{
	Field *aux = (*posicao)->arqs->campos;
	int pos = 5;
	float valor;
	char string[50];
	while(aux != NULL)
	{
		gotoxy(10,pos);
		printf("%s",aux->fieldname);
		if(aux->type == 'N')
		{
			gotoxy(25,pos);
			scanf("%f",&valor);
			Type *caixa = (Type*)malloc(sizeof(Type));
			caixa->terminal = aux->type;
			caixa->prox = NULL;
			caixa->no.number = valor;
			if(aux->pdados == NULL)
			{
				aux->pdados = caixa;
			}
			else
			{
				while(aux->pdados->prox != NULL)
				{
					aux->pdados = aux->pdados->prox;
				}
				aux->pdados->prox = caixa;
			}
		}
		else
		{
			gotoxy(25,pos);
			fflush(stdin);
			gets(string);
			Type *caixa = (Type*)malloc(sizeof(Type));
			caixa->terminal = aux->type;
			caixa->prox = NULL;
			if(caixa->terminal == 'D')
				strcpy(caixa->no.date,string);
			else if(caixa->terminal == 'L')
				caixa->no.logical = string[0];
			else if(caixa->terminal == 'C')
				strcpy(caixa->no.character,string);
			else if(caixa->terminal == 'M')
				strcpy(caixa->no.memo,string);
			if(aux->pdados == NULL)
			{
				aux->pdados = caixa;
			}
			else
			{
				while(aux->pdados->prox != NULL)
				{
					aux->pdados = aux->pdados->prox;
				}
				aux->pdados->prox = caixa;
			}
		}
		pos++;
		aux = aux->prox;
	}
	gotoxy(10,pos);
	printf("Registro Inserido. Pressione qualquer tecla para continuar.");
}

void comando(char *cmd,Unidade *und,Unidade **posicao)
{
	if(set_default_to(cmd))
	{
		char *pch;
		pch = strtok(cmd," ");
		int i = 0;
		char *letra[5];
		while(pch != NULL)
		{
			letra[i] = pch;
			i++;
			pch = strtok(NULL," ");
		}
		*posicao = buscaDir(und,letra[3]);
	}
	else if(create(cmd))
	{
		char *pch;
		char arquivo[100];
		pch = (char*)memchr(cmd,' ',strlen(cmd));
		int i = pch-cmd+1;
		int j = 0;
		while(i < strlen(cmd))
		{
			arquivo[j] = cmd[i];
			i++;
			j++;
		}
		arquivo[j] = '\0';
		if(*posicao != NULL)
		{
			if(strcmp(arquivo,"") != 0)
			{
				criarArquivo(arquivo,&*posicao);
				limpatela();
				desenhaBorda();
				gotoxy(80,27);
				printf("Created %s in %s",arquivo,(*posicao)->und);
				gotoxy(80,28);
				printf("Press Any Key to Continue");
				getch();
				limpaParcial();
				desenhaCreate();
				typeFields(&*posicao,arquivo);
			}
			else
			{
				gotoxy(80,27);
				printf("Digite um nome valido!");
				getch();
			}
			
		}
		else
		{
			gotoxy(80,28);
			printf("Set Default Disk Letter First");
			getch();
		}
	}
	else if(stricmp("DIR",cmd) == 0)
	{
		if(*posicao != NULL)
		{
			limpatela();
			desenhaBorda();
			Unidade *aux = *posicao;
			Dir(aux);
			getch();
			telainicial();
		}
		
	}
	else if(Use(cmd))
	{
		char *pch;
		pch = strtok(cmd," ");
		char *palavra[3];
		int i = 0;
		while(pch != NULL)
		{
			palavra[i] = pch;
			i++;
			pch = strtok(NULL," ");
		}
		if((*posicao) != NULL)
		{
			if((*posicao)->arqs != NULL)
			{
				(*posicao)->arqs = buscarDatabase(&*posicao,&*palavra[1]);
				textcolor(BLACK);
				gotoxy(80,28);
				printf("%d",(*posicao)->arqs);
			}
			else
			{
				gotoxy(80,28);
				printf("Database nao encontrada!");
			}
				
		}
		else
		{
			gotoxy(80,28);
			printf("Unidade nao selecionada!");
		}
		getch();
	}
	else if(stricmp("LIST STRUCTURE",cmd) == 0)
	{
		gotoxy(80,28);
		if((*posicao) != NULL)
		{
			if((*posicao)->arqs != NULL)
			{
				limpatela();
				desenhaBorda();
				imprimeStructure(*posicao);
			}
			else
			{
				gotoxy(80,28);
				printf("Database nao selecionada!");
			}
		}
		else
		{
			gotoxy(80,28);
			printf("Unidade nao selecionada!");
		}
		getch();
		telainicial();
	}
	else if(stricmp("APPEND",cmd) == 0)
	{
		gotoxy(80,28);
		if((*posicao) != NULL)
		{
			if((*posicao)->arqs != NULL)
			{
				limpatela();
				desenhaBorda();
				insereRegistro(&*posicao);
			}
			else
			{
				gotoxy(80,28);
				printf("Database nao selecionada!");
			}
		}
		else
		{
			gotoxy(80,28);
			printf("Unidade nao selecionada!");
		}
		getch();
		telainicial();
	}
	else if(stricmp("LIST",cmd) == 0)
	{
		gotoxy(80,28);
		if((*posicao) != NULL)
		{
			if((*posicao)->arqs != NULL)
			{
				limpatela();
				desenhaBorda();
				ExibeRegistros(*posicao);
			}
			else
			{
				gotoxy(80,28);
				printf("Database nao selecionada!");
			}
		}
		else
		{
			gotoxy(80,28);
			printf("Unidade nao selecionada!");
		}
		getch();
		telainicial();
	}
}

void limpatela()
{
	int i,j;
	for(i = 0;i < 31;i++)
	{
		for(j = 0;j < 121;j++)
		{
			gotoxy(j,i);
			textcolor(15);
			printf("%c",219);
		}	
	}
}

void deslMaximizar()
{
	HWND hwnd = GetConsoleWindow();
	DWORD style = GetWindowLong(hwnd, GWL_STYLE);
	style &= ~WS_MAXIMIZEBOX;
	SetWindowLong(hwnd, GWL_STYLE, style);
	SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE|SWP_FRAMECHANGED);
}

void escreveComando(char *cmd,char *desc,char *use,int vpos)
{
	gotoxy(6,vpos);
	printf("%s",cmd);
	gotoxy(30,vpos);
	printf("%s",desc);
	gotoxy(85,vpos);
	printf("%s",use);
}

void escreveCabecalho(char *args,char *args2,char *args3,char *args4)
{
	gotoxy(6,4);
	printf("%s",args);
	gotoxy(10,6);
	printf("%s",args2);
	gotoxy(37,6);
	printf("%s",args3);
	gotoxy(90,6);
	printf("%s",args4);
}

void limpaParcial()
{
	int i,j;
	for(i = 10;i < 119;i++)
	{
		gotoxy(i,27);
		textcolor(15);
		printf("%c",219);
		gotoxy(i,28);
		printf("%c",219);
	}
	textcolor(0);
}

char *commandType(char *cmd)
{
	gotoxy(5,27);
	printf("Type Command: ");
	fflush(stdin);
	gets(cmd);
	return cmd;
}

void commandList()
{
	escreveCabecalho("LIST OF COMMANDS","COMMAND","DESCRIPTION","USAGE EXAMPLE");
	escreveComando("SET DEFAULT TO","Select Current Disk Letter","SET DEFAULT TO C:",7);
	escreveComando("CREATE","Create New Database File","CREATE DBNAME.DBF",8);
	escreveComando("DIR","Show All Files From Current Disk Letter","DIR",9);
	escreveComando("QUIT","Quit from dBase","QUIT",10);
	escreveComando("USE","Open Database File","USE DBNAME.DBF",11);
	escreveComando("LIST STRUCTURE","List Structure from Selected .DBF File","LIST STRUCTURE",12);
	escreveComando("APPEND","Insert New Register in Last Position","APPEND",13);
	escreveComando("LIST","List All Fields","LIST",14);
	escreveComando("CLEAR","Clear Screen and Return to Main Menu","CLEAR",15);
	escreveComando("LOCATE","List Fields With Filter","LOCATE FOR NAME = 'Joao da Silva'",16);
	escreveComando("GOTO","Goto Determined Number Record","GOTO 2",17);
	escreveComando("DISPLAY","Display Current Register","DISPLAY",18);
	escreveComando("EDIT","Edit Current Register","EDIT",19);
	escreveComando("DELETE","Mark Current Register or All to Deletion","DELETE OR DELETE ALL",20);
	escreveComando("RECALL","Reactive Record or All Records Marked for Deletion","RECALL OR RECALL ALL",21);
	escreveComando("SET DELETED","Show or Hide Register Marked for Deletion","SET DELETED ON OR SET DELETED OFF",22);
	escreveComando("PACK","Physically Deletes Records Marked for Deletion","PACK",23);
	escreveComando("ZAP","Delete All Registers of .DBF File in Use","ZAP",24);
}

void desenhaBorda()
{
	int i,j;
	textcolor(0);
	textbackground(15);
	gotoxy(2,1);
	printf("%c",201);
	for(i = 3;i < 119;i++)
	{
		gotoxy(i,1);
		printf("%c",205);
	}
	gotoxy(119,1);
	printf("%c",187);
	for(i = 2;i < 30;i++)
	{
		gotoxy(119,i);
		printf("%c",186);
	}
	gotoxy(119,30);
	printf("%c",188);
	for(i = 118;i > 2;i--)
	{
		gotoxy(i,30);
		printf("%c",205);
	}
	gotoxy(2,30);
	printf("%c",200);
	for(i = 29;i > 1;i--)
	{
		gotoxy(2,i);
		printf("%c",186);
	}
	gotoxy(50,2);
	printf("dBase v1.0b");
	for(i = 49;i < 63;i++)
	{
		gotoxy(i,3);
		printf("%c",205);
	}
}

void telainicial()
{
	limpatela();
	desenhaBorda();
	commandList();
}

void Startup()
{
	int i,j;
	char cmd[100];
	Unidade *und = NULL;
	Unidade *pos = NULL;
	createDir(&und,"C:");
	createDir(&und,"D:");
	deslMaximizar();
	telainicial();
	strcpy(cmd," ");
	while(strcmp(cmd,"QUIT") != 0)
	{
		strcpy(cmd,commandType(cmd));
		comando(cmd,und,&pos);
		limpaParcial();
	}
}

