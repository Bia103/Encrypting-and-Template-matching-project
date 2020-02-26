#include <stdio.h>
#include <stdlib.h>
#include<math.h>

typedef struct
{
    int x,y,z;
    double cor;
} coord;
int max(int x,int y)
{
    if(x<y)
        return y;
    return x;
}
int min(int x,int y)
{
    if(x<y)
        return x;
    return y;
}
double intersectia_dreptunghiurilor(coord i,coord j,int w,int h)
{
    double aria=(double)(w*h);
    double aria_i=(min(i.x-w/2,j.x-w/2)-max(i.x+w/2,j.x+w/2))*(min(i.y-h/2,j.y-h/2)-max(i.y+h/2,j.y+h/2));
    return aria_i/(aria*2-aria_i);
}
void non_maxime(coord **f,int *nr,int w,int h)
{
    for(int i=0; i<(*nr)-1; i++)
    {
        for(int j=i+1; j<(*nr); j++)
        {
            if((*f)[i].x!=-1&&(*f)[j].x!=-1)
                if(intersectia_dreptunghiurilor((*f)[i],(*f)[j],w,h)>0.2)
                {
                    (*f)[j].x=-1;
                    (*f)[j].y=-1;
                    (*f)[j].z=-1;
                    (*f)[j].cor=-1;
                }
        }
    }
}
double med_val_intens_grayscale(int i,int j,unsigned int *a,int latime,int inaltime,int w,int h)
{
    int n,m;
    unsigned int x=0,z=255;
    double f=0.0,in=0.0;
    for(n=i-h/2; n<=i+(h)/2; n++)
    {
        for(m=j-w/2; m<=j+w/2; m++)
        {
            if(n<0||m<0||n>=inaltime||m>=latime)
                f=f+0.0;
            else
            {
                f=(double)(f+(double)(a[n*latime+m]&z));
                in++;
            }
        }
    }
    f=(double)f/(double)in;
    return f;
}
double deviatia_standard_f(int i,int j,unsigned int *a,int latime,int inaltime,int w,int h,double fi)
{
    int n,m;
    double of=0.0,in=0.0,in2=w*h;
    unsigned int x=0,z=255;
    for(n=i-h/2; n<=i+(h)/2; n++)
    {
        for(m=j-w/2; m<=j+w/2; m++)
        {
            if(n<0||m<0||n>=inaltime||m>=latime)
                of=of+0.0;
            else
            {
                of=(double)(((double)(a[n*latime+m]&z)-fi)*((double)(a[n*latime+m]&z)-fi)+of);
                in++;
            }
        }
    }
    of=sqrt((double)of/(double)(in-1));
    return of;
}
void template_matching(char *imag,char *sablon,double ps,unsigned int **a,unsigned int **s,int w,int h,int latime,int inaltime,coord **g,int c,int *nr_g)
{
    int i,j,cont=0;
    printf("\n");
    double os=0.0,Sm=0.0,S=0.0,ofi=0.0,cor=0.0,n=(double)w*h;
    unsigned int z=255;
    for(i=0; i<h; i++)
        for(j=0; j<w; j++)
        {
            Sm=Sm+(double)((*s)[i*w+j]&z);
        }
    Sm=(double)Sm/(double)n;
    for(i=0; i<h; i++)
        for(j=0; j<w; j++)
        {
            S=((double)((*s)[i*w+j]&z)-Sm)*((double)((*s)[i*w+j]&z)-Sm)+S;
        }
    os=sqrt((double)S/(double)(n-1));
    for(i=0; i<inaltime; i++)
    {
        for(j=0; j<latime; j++)
        {
            int ms=0,ns=0;
            double n=0.0;
            cor=0.0;
            double fim=med_val_intens_grayscale(i,j,(*a),latime,inaltime,w,h);
            double of=deviatia_standard_f(i,j,(*a),latime,inaltime,w,h,fim);
            double f=0.0,rs=0.0;
            if(of!=0.0)
            {
                for(int k=i-h/2; k<=i+(h)/2; k++)
                {
                    for(int m=j-w/2; m<=j+(w)/2; m++)
                    {
                        if(k<0||m<0||k>=inaltime||m>=latime)
                        {
                            f=0.0;
                            rs=0.0;
                        }
                        else
                        {
                            f=(double)((*a)[k*latime+m]&z);
                            n++;
                            double aux=(double)(1/(os*of));
                            rs=(double)((*s)[ms*w+ns]&z);
                            cor=(double)(cor+((double)(f-fim)*(double)(rs-Sm)*aux));
                        }
                        ns++;
                    }
                    ns=0;
                    ms++;
                }
                cor=(double)cor/(double)n;//
                if(cor>=ps)
                {
                    (*nr_g)++;
                    coord *aux=(coord*)realloc((*g),sizeof(coord)*(*nr_g));
                    aux[(*nr_g)-1].x=i;
                    aux[(*nr_g)-1].y=j;
                    aux[(*nr_g)-1].z=c;
                    aux[(*nr_g)-1].cor=cor;
                    (*g)=aux;

                }
            }
        }

    }
}
void colorez(char *imagine,int i,int j,unsigned int culoare,unsigned int **a,int h,int w,int latime,int inaltime)
{
    int m=j-w/2,n;
    n=i-h/2;
    if(n>=0&&n<inaltime)
    {

        for(m=j-w/2; m<=j+w/2; m++)
        {
            if(m<latime&&m>=0&&n*latime+m<latime*inaltime)
                (*a)[n*latime+m]=culoare;
        }
    }
    else if(n<0)
    {
        n=0;
        for(m=j-w/2; m<=j+w/2; m++)
        {
            if(m<latime&&m>=0&&n*latime+m<latime*inaltime)
                (*a)[n*latime+m]=culoare;
        }
    }
    n=i+h/2;
    if(n<inaltime&&n>=0)
        for(m=j-w/2; m<=j+w/2; m++)
        {
            if(m<latime&&m>=0&&n*latime+m<latime*inaltime)
                (*a)[n*latime+m]=culoare;
        }
    else if(n>=inaltime)
    {
        n=inaltime-1;
        for(m=j-w/2; m<=j+w/2; m++)
        {
            if(m<latime&&m>=0&&n*latime+m<latime*inaltime)
                (*a)[n*latime+m]=culoare;
        }
    }
    n=j-w/2;
    if(n>=0&&n<latime)
        for(m=i-h/2; m<=i+h/2; m++)
        {
            if(m<inaltime&&m>=0&&m*latime+n<latime*inaltime)
                (*a)[(m)*latime+n]=culoare;
        }
    n=j+w/2;
    if(n>=0&&n<latime)
        for(m=i-h/2; m<=i+h/2; m++)
        {
            if(m<inaltime&&m>=0&&m*latime+n<latime*inaltime)
                (*a)[(m)*latime+n]=culoare;
        }
}
void grayscale_image(char* nume_fisier_sursa,char* nume_fisier_destinatie)
{
    printf("\n");
    FILE *fin, *fout;
    unsigned int dim_img, latime_img, inaltime_img;
    unsigned char pRGB[3], header[54], aux;
    printf("nume_fisier_sursa = %s \n",nume_fisier_sursa);
    fin = fopen(nume_fisier_sursa, "rb");
    if(fin == NULL)
    {
        printf("nu am gasit imaginea sursa din care citesc");
        return;
    }
    fout = fopen(nume_fisier_destinatie, "wb+");
    fseek(fin, 2, SEEK_SET);
    fread(&dim_img, sizeof(unsigned int), 1, fin);
    printf("Dimensiunea imaginii in octeti: %u\n", dim_img);

    fseek(fin, 18, SEEK_SET);
    fread(&latime_img, sizeof(unsigned int), 1, fin);
    fread(&inaltime_img, sizeof(unsigned int), 1, fin);
    printf("Dimensiunea imaginii in pixeli (latime x inaltime): %u x %u\n",latime_img, inaltime_img);
    fseek(fin,0,SEEK_SET);
    unsigned char c;
    while(fread(&c,1,1,fin)==1)
    {
        fwrite(&c,1,1,fout);
        fflush(fout);
    }
    fclose(fin);
    int padding;
    if(latime_img % 4 != 0)
        padding = 4 - (3 * latime_img) % 4;
    else
        padding = 0;
    printf("padding = %d \n",padding);
    fseek(fout, 54, SEEK_SET);
    int i,j;
    for(i = 0; i < inaltime_img; i++)
    {
        for(j = 0; j < latime_img; j++)
        {
            fread(pRGB, 3, 1, fout);
            aux = 0.299*pRGB[2] + 0.587*pRGB[1] + 0.114*pRGB[0];
            pRGB[0] = pRGB[1] = pRGB[2] = aux;
            fseek(fout, -3, SEEK_CUR);
            fwrite(pRGB, 3, 1, fout);
            fflush(fout);
        }
        fseek(fout,padding,SEEK_CUR);
    }
    fclose(fout);
}
void xorshift32(unsigned int seed,unsigned int **r,unsigned int n)
{
    int i=1;
    unsigned int x =seed;
    for(unsigned int k=0; k<n-1; k++)
    {
        x ^= x << 13;
        x ^= x >> 17;
        x ^= x << 5;
        (*r)[i]=x;
        i++;
    }
}
void decriptare(char *nume_fis_sursa,char *cale,char *cod_secret,int *inaltime,int *latime,unsigned int **a,unsigned char **v)
{
    FILE *sk=fopen(cod_secret,"r");
    unsigned int *r=(unsigned int*)malloc(((*inaltime)*(*latime)*2)*sizeof(unsigned int)),x;
    unsigned int *p=(unsigned int*)malloc((*latime)*(*inaltime)*sizeof(unsigned int));
    fscanf(sk,"%u",&r[0]);
    x=(*latime)*(*inaltime)*2;
    xorshift32(r[0],&r,x);
    int i;
    unsigned int sv;
    fscanf(sk,"%u",&sv);
    int c=(*latime)*(*inaltime)*2-1;
    for(i=(*latime)*(*inaltime)-1; i>=1; i--)
    {
        (*a)[i]=(((*a)[i-1]^(*a)[i])^r[c]);
        c--;
    }
    (*a)[0]=(((*a)[0]^sv)^r[c]);
    unsigned int *o=(unsigned int*)malloc(((*inaltime)*(*latime)-1)*sizeof(unsigned int)+1);
    for(i=0; i<(*inaltime)*(*latime); i++)
        o[i]=i;
    i=1;
    for(int k=(*latime)*(*inaltime)-1; k>=1; k--)
    {
        int ra=r[i]%(k+1);
        int aux=o[ra];
        o[ra]=o[k];
        o[k]=aux;
        i++;
    }
    for(i=0; i<(*inaltime)*(*latime); i++)
    {
        p[i]=(*a)[o[i]];
    }
    *a=p;
    i=1;
    fclose(sk);
}
void criptare(char *nume_fis_sursa,char *cale,char *cod_secret,int *inaltime,int *latime,unsigned int **a,unsigned char **v)
{
    FILE *sk=fopen(cod_secret,"r");
    unsigned int *r=(unsigned int*)malloc(((*inaltime)*(*latime)*2)*sizeof(unsigned int)),x;
    unsigned int *p=(unsigned int*)malloc((*latime)*(*inaltime)*sizeof(unsigned int));
    fscanf(sk,"%u",&r[0]);
    x=(*latime)*(*inaltime)*2;
    xorshift32(r[0],&r,x);
    int i;
    unsigned int *o=(unsigned int*)malloc(((*inaltime)*(*latime))*sizeof(unsigned int));
    for(i=0; i<(*inaltime)*(*latime); i++)
        o[i]=i;
    i=1;
    for(int k=(*latime)*(*inaltime)-1; k>=1; k--)
    {
        int ra=r[i]%(k+1);
        int aux=o[ra];
        o[ra]=o[k];
        o[k]=aux;
        i++;

    }
    for(i=0; i<(*inaltime)*(*latime); i++)
    {
        p[o[i]]=(*a)[i];
    }
    *a=p;
    unsigned int sv;
    fscanf(sk,"%u",&sv);
    int c=(*latime)*(*inaltime);
    (*a)[0]=(((*a)[0]^sv)^r[c]);
    c++;

    for(i=1; i<(*latime)*(*inaltime); i++)
    {
        (*a)[i]=(((*a)[i-1]^(*a)[i])^r[i+(*latime)*(*inaltime)]);
        c++;
    }
    fclose(sk);
}
void incarc_o_imagine_in_memorie(char* cale,unsigned char *v,unsigned int *a,unsigned int n,unsigned int m)
{
    FILE *f=fopen(cale,"wb");
    //printf("\n");
    if(f==NULL)
    {
        printf("Fisier invalid");
        return ;
    }
    else
        printf("Fisier valid ");
    int i,j;
    for(j=0; j<=53; j++)
    {
        fwrite(&v[j],1,1,f);
        fflush(f);
    }
    int p=(m)*(n);
    for(i=0; i<m; i++)
    {
        int  z=p-(i+1)*(n);
        for(j=0; j<n; j++)
        {
            fwrite(&a[z],1,3,f);
            z++;
            fflush(f);
        }
    }
    fclose(f);

}
void incarc_imaginea_liniarizata(char* nume_fis_sursa,unsigned int **a,unsigned char**v,unsigned int *inaltime,unsigned int *latime)
{
    FILE *fin=fopen(nume_fis_sursa,"rb");
    long z=0;
    if(fin==NULL)
    {
        printf("Fisier invalid");
        return ;
    }
    else
        printf("Fisier valid ");
    fseek(fin,18,SEEK_SET);
    fread(&(*latime),sizeof(unsigned int),1,fin);
    fread(&(*inaltime),sizeof(unsigned int),1,fin);
    *a=(unsigned int*)malloc((*latime)*(*inaltime)*sizeof(unsigned int));
    *v=(unsigned char*)malloc(54*sizeof(unsigned char));
    fseek(fin,0,SEEK_SET);
    unsigned int c;
    int i=0,j=0;
    int padding;
    if((*latime) % 4 != 0)
        padding = 4 - (3 * (*latime)) % 4;
    else
        padding = 0;
    for(j=0; j<=53; j++)
    {
        fread(&(*v)[j],1,1,fin);
    }
    fseek(fin,54,SEEK_SET);
    int k=(*inaltime)*(*latime);
    for(i=0; i<(*inaltime); i++)
    {
        int z=k-(i+1)*(*latime);
        for(j=0; j<(*latime); j++)
        {
            fread(&c,1,3,fin);
            (*a)[z]=c;
            z++;
        }
        fseek(fin,padding,SEEK_CUR);
    }
    fseek(fin,0,SEEK_SET);
    fclose(fin);

}
void test_chi(unsigned int *a,int *latime,int *inaltime)
{
    int c=0;
    float ci=0,f=(*inaltime)*(*latime)/256;
    unsigned int x=255;
    for(int s=0; s<=2; s++)
    {
        for(unsigned int i=0; i<=255; i++)
        {
            float cont=0;
            for(int j=0; j<(*latime); j++)
                for(int k=0; k<(*inaltime); k++)
                {
                    if((a[c]&x)==(i<<(8*s)))
                        cont++;
                    c++;
                }
            ci=(cont-f)*(cont-f)/f+ci;
            c=0;
        }
        printf("%f  ",ci);
        x=(x<<8);
        ci=0;
    }
}
int cmp(const void *a,const void *b)
{
    coord X,Y;
    X=*(coord*)a;
    Y=*(coord*)b;
    if(X.cor>Y.cor)
        return -1;
    if(X.cor<Y.cor)
        return 1;
    return 0;
}
int main()
{
    unsigned long long z=0;
    unsigned int *a,*s,y=255,**p;
    unsigned char *v,*s1;
    unsigned int inaltime,latime,w,h;
    char nume_fis_sursa[101],cale[101],cod_secret[101];
    printf("Numele fisierului care contine imaginea ce trebuie criptata: ");
    fgets(nume_fis_sursa, 101, stdin);
    nume_fis_sursa[strlen(nume_fis_sursa) - 1] = '\0';
    printf("Locul in care trebuie incarcata imaginea dupa criptare : ");
    fgets(cale, 101, stdin);
    cale[strlen(cale) - 1] = '\0';
    printf("Numele fisierului care contine cheia secreta: ");
    fgets(cod_secret, 101, stdin);
    cod_secret[strlen(cod_secret) - 1] = '\0';
    printf("Statutul fisierului care contine imaginea ce va fi criptata : ");
    incarc_imaginea_liniarizata(nume_fis_sursa,&a,&v,&inaltime,&latime);
    printf("\nValorile obtinute in urma testului chi inainte de criptare : ");
    test_chi(a,&latime,&inaltime);
    criptare(nume_fis_sursa,cale,cod_secret,&inaltime,&latime,&a,&v);
    printf("\nValorile obtinute in urma testului chi dupa criptare : ");
    test_chi(a,&latime,&inaltime);
    printf("\nStatutul fisierului in care va fi incarcata imaginea criptata : ");
    incarc_o_imagine_in_memorie(cale,v,a,latime,inaltime);

    printf("\nNumele fisierului care contine imaginea ce trebuie decriptata: ");
    fgets(nume_fis_sursa, 101, stdin);
    nume_fis_sursa[strlen(nume_fis_sursa) - 1] = '\0';
    printf("Locul in care trebuie incarcata imaginea dupa decriptare : ");
    fgets(cale, 101, stdin);
    cale[strlen(cale) - 1] = '\0';
    printf("Numele fisierului care contine cheia secreta: ");
    fgets(cod_secret, 101, stdin);
    cod_secret[strlen(cod_secret) - 1] = '\0';
     // printf(" %s \n",cale);
    decriptare(nume_fis_sursa,cale,cod_secret,&inaltime,&latime,&a,&v);
    printf("Statutul fisierului care contine imaginea dupa decriptare : ");
    incarc_o_imagine_in_memorie(cale,v,a,latime,inaltime);
    printf("\nNumele fisierului care contine imaginea careia i se va aplica operatia de template matching : ");
    fgets(cale, 101, stdin);
    cale[strlen(cale) - 1] = '\0';
    grayscale_image(cale,"numere.bmp");
    incarc_imaginea_liniarizata("numere.bmp",&a,&v,&inaltime,&latime);
    unsigned int culoare=(y<<16);
   // printf("\n Numarul de sabloane care vor fi folosite : ");
    int nr_x=10;
   // scanf("%d",&nr_x);//printf("Da");
    int c,nr_f=0,nr=0;
    float ps=0.5;
        coord *f=(coord *)malloc(sizeof(coord));
    for(int i=0;i<nr_x;i++)
    {
        printf("\n Sablon %d :",i+1);
        char cif[101];
        fgets(cif, 101, stdin);
        cif[strlen(cif) - 1] = '\0';
        switch(i)
        {
        case 0:
            culoare=(255<<16);
            break;
        case 1:
            culoare=(255<<16)+(255<<8);
            break;
        case 2:
            culoare=(255<<8);
            break;
        case 3:
            culoare=255+(255<<8);
            break;
        case 4:
            culoare=255+(255<<16);
            break;
        case 5:
            culoare=255;
            break;
        case 6:
            culoare=(((192<<16)|(192<<8))|(192));
            break;
        case 7:
            culoare=(((255<<16)|(140<<8)));
            break;
        case 8:
            culoare=((128<<16)|(128));
            break;
        case 9:
            culoare=((128<<16));
            break;
        }
        printf(" %u ",culoare);
        grayscale_image(cif,"cif.bmp");
        printf("Statutul fisierului din care se citeste sablonul : ");
        incarc_imaginea_liniarizata(cif,&s,&s1,&h,&w);
         template_matching("numere.bmp","cif.bmp",ps,&a,&s,w,h,latime,inaltime,&f,culoare,&nr_f);
    }
    incarc_imaginea_liniarizata(cale,&a,&v,&inaltime,&latime);

   qsort(f,nr_f,sizeof(coord),cmp);
    printf("\nNumarul detectiilor inainte de eliminarea non-maximelor : %d ",nr_f);
    non_maxime(&f,&nr_f,w,h);
    int nr1=0;
    for(int i=0; i<nr_f; i++)
    {
        if(f[i].x!=-1)
        {
            colorez(cale,f[i].x,f[i].y,f[i].z,&a,h,w,latime,inaltime);
            nr1++;
        }
    }
    printf("\nNumarul detectiilor dupa eliminarea non-maximelor : %d ",nr1);
  printf("\nStatutul fisierului in urma template-matching: ");
    incarc_o_imagine_in_memorie(cale,v,a,latime,inaltime);
    free(a);
    free(v);
    free(s);
    free(s1);
    return 0;
}
// Proiect realizat de studenta Furculesteanu Bianca de la grupa 141
