#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATABASE_PATH = "database.txt";

typedef struct {
    int hora;
    int minuto;
} Horario;

typedef struct {
    int codigo;
    char* descricao;
    Horario hora_inicio;
    Horario hora_fim;
} Compromisso;

typedef struct {
    Compromisso* vetor;
    int qtdade;
    int capacidade;
} ArrayList;

typedef struct {
    ArrayList* compromissos;
} Calendario;

// Funções da lista de compromissos

ArrayList* inicializar(int capacidade);
Compromisso obterElementoEmPosicao(ArrayList* lista, int posicao);
void duplicarCapacidade(ArrayList* lista);
void inserirElementoNoFim(ArrayList* lista, Compromisso novoCompromisso);
void inserirElementoEmPosicao(ArrayList* lista, Compromisso novoCompromisso, int posicao);
void atualizarElemento(ArrayList* lista, Compromisso compromissoAtualizado, int posicao);
void removerElementoNoFim(ArrayList* lista);
void removerElementoEmPosicao(ArrayList* lista, int posicao);
void exibirLista(ArrayList* lista);

// Funções da agenda
Compromisso* carrega_agenda(int* tamanho);

int main(void){
    int* qntdade_compromissos;
    carrega_agenda(qntdade_compromissos);
}

Compromisso* carrega_agenda(int* tamanho){
    
    Calendario calendario;

    int total_linhas = 0;

    FILE* database = fopen("database.txt", "r");

    // calendario.compromissos = inicializar(*tamanho);
    char palavra[255];
    char word[255];
    char *path;
    char line[255] = {0};
    unsigned int line_count = 0;

    if (!database){
        printf("Deu ruim na abertura do arquivo!");
        return 0;
    }

    int cont = 0;
    int index_compromisso = 0;
    int cont_total_eventos = 0;
    while (fgets(line, 255, database)) {

        char* mLine = calloc(255, sizeof(char));

        strcpy(mLine, line);
        int qtd_total_eventos;
        if(cont == 0) {
            qtd_total_eventos = atoi(mLine);
            calendario.compromissos = inicializar(qtd_total_eventos);
            *tamanho = qtd_total_eventos;
        } else if(cont > 1){
            //printf("%d - %s", index_compromisso, mLine);
            int codigo;
            char* descricao = calloc(255, sizeof(char));
            char* periodo = calloc(255, sizeof(char));
            Horario horarioInicio;
            Horario horarioFim;
            Compromisso compromisso;
            switch (index_compromisso){
            case 0:
                codigo =  atoi(mLine);
                break;
            case 1:
                strcpy(descricao, mLine);
                break;
            case 2:
                strcpy(periodo, mLine);
                char* horarios_str;

                horarios_str = strtok(periodo, " ");
                char* inicio_fim[2];

                int cont_inicio_fim = 0;

                while(horarios_str != NULL){
                    inicio_fim[cont_inicio_fim] = horarios_str;
                    horarios_str = strtok(NULL, " ");
                    cont_inicio_fim++;
                }
                //printf("%s - %s\n", inicio_fim[0], inicio_fim[1]);

                char* inicio = strtok(inicio_fim[0], ":");             
                int cont_inicio = 0;
                while(inicio != NULL){
                    if(cont_inicio == 0){
                        horarioInicio.hora = atoi(inicio);
                    } else {
                        horarioInicio.minuto = atoi(inicio);
                    }
                    inicio = strtok(NULL, ":");
                    cont_inicio++;
                }

                char* fim = strtok(inicio_fim[1], ":");
                int cont_fim = 0;
                while(fim != NULL){
                    if(cont_fim == 0){
                        horarioFim.hora = atoi(fim);
                    } else {
                        horarioFim.minuto = atoi(fim);
                    }
                    fim = strtok(NULL, ":");
                    cont_fim++;
                }
                break;
            default:
                break;
            }

            index_compromisso++;
            if(index_compromisso > 2){
                index_compromisso = 0;
                compromisso.codigo = codigo;
                //strcpy(compromisso.descricao, descricao);
                compromisso.descricao = descricao;
                compromisso.hora_inicio = horarioInicio;
                compromisso.hora_fim = horarioFim;
                calendario.compromissos->vetor[qtd_total_eventos] = compromisso;

                printf("---------------------------------\n");
                printf("Código: %d\n", calendario.compromissos->vetor[qtd_total_eventos].codigo);
                printf("Descrição: %s\n", compromisso.descricao);
                printf("Início: %d:%d\n", compromisso.hora_inicio.hora, compromisso.hora_inicio.minuto);
                printf("Fim: %d:%d\n", compromisso.hora_fim.hora, compromisso.hora_fim.minuto);
                printf("---------------------------------\n");
            }
            cont_total_eventos++;  
        }
        
        cont++;
    }
    
    fclose(database);

    return calendario.compromissos->vetor;
}

ArrayList* inicializar(int capacidade) {
    ArrayList* lista = (ArrayList*)malloc(sizeof(ArrayList));
    lista->vetor = (Compromisso*)calloc(capacidade, sizeof(Compromisso));
    lista->qtdade = 0;
    lista->capacidade = capacidade;
    return lista;
}

Compromisso obterElementoEmPosicao(ArrayList* lista, int posicao) {
    if (posicao >= 0 && posicao < lista->qtdade) {
        return lista->vetor[posicao];
    }
}

void duplicarCapacidade(ArrayList* lista) {
    lista->capacidade *= 2;
    Compromisso* aux = lista->vetor;
    if ((lista->vetor = (Compromisso*)realloc(lista->vetor, sizeof(Compromisso) * lista->capacidade)) == NULL) {
        free(aux);
    }
}

void inserirElementoNoFim(ArrayList* lista, Compromisso novoCompromisso) {
    if (lista->qtdade == lista->capacidade) {
        duplicarCapacidade(lista);
    }
    lista->vetor[lista->qtdade] = novoCompromisso;
    lista->qtdade += 1;
}

void inserirElementoEmPosicao(ArrayList* lista, Compromisso novoCompromisso, int posicao) {
    if (posicao >= 0 && posicao <= lista->qtdade) {
        if (lista->qtdade == lista->capacidade) {
            duplicarCapacidade(lista);
        }
        for (int i = lista->qtdade; i > posicao; i--) {
            lista->vetor[i] = lista->vetor[i - 1];
        }
        lista->vetor[posicao] = novoCompromisso;
        lista->qtdade++;
    }
}

void atualizarElemento(ArrayList* lista, Compromisso compromissoAtt, int posicao) {
    if (posicao >= 0 && posicao <= lista->qtdade) {
        lista->vetor[posicao] = compromissoAtt;
    }
}

void removerElementoNoFim(ArrayList* lista) {
    lista->qtdade--;
}

void removerElementoEmPosicao(ArrayList* lista, int posicao) {
    if (posicao >= 0 && posicao <= lista->qtdade) {
        for (int i = posicao; i < lista->qtdade; i++) {
            lista->vetor[i] = lista->vetor[i + 1];
        }
        lista->qtdade--;
    }
}

/*void exibirLista(ArrayList* lista) {
    printf("[");
    for (int i = 0; i < lista->qtdade; i++) {
        printf("%d", lista->vetor[i]);
        if (i < lista->qtdade - 1) {
            printf(", ");
        }
    }
    printf("]");
}*/
