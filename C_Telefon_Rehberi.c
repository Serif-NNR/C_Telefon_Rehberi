/*
 * CopyLeft  2017 Serif Inanir
 * Created 16.10.2017-15.47
 * 
 * C_Telefon_Rehberi is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * C_Telefon_Rehberi is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
*/

/* 		PROGRAM GENEL BILGISI  										*\	
  Isaretcilerle ve Struct larla yapilmis rehber uygulamasidir.
  Tek yonlu bagli liste kullanilarak kayit islemleri yapilir.
  Rehber dizisinin herhangi bir indexi, kendisine atanmis olan
  harfi ve o harf ile baslayan ilk kaydin adresini de tutar. 
  Kayit sirasina uygun, alfabetik isim sirasina uygun ve alfabetik
  soyisim sirasina uygun olarak, kayit edilecek veri 3 ayri
  yerde tutulur. Program, isim ve soyisim siralamsini, kabarcik
  siralama ile yapmaktadir. 
  
\*																	*/

/*
 *	 NOT: Olmayan bir kaydi silmeye calisinca, hic kayit yapilmamisken isme ve soyisme
 *		  gore siralamaya calisinca program hata veriyor.
 */															



#include <stdio.h>      								// NULL ifadesi burada 0 (0 , OLL ve ((void*) 0)) olarak tanimlanmistir.
#include <stdlib.h>										// exit(1) fonksiyonu buradadir.
#include <ctype.h>										// tolower() fonksiyonu icin.
#include <string.h>										// strcpy , strcmp fonksiyonlari icin.

/* TODO (#0#): Struct Tanimlamalari */
typedef struct Kayit{
	char ad[50];
	char soyad[50];
	int telNo;
	struct Kayit *sonrakiKayit;
}KAYIT;

typedef struct Rehber{
	char ilkHarf;
	struct Kayit *ilkKayitPtr;
}REHBER;

REHBER rehber[27];										// Global rehber dizisi.

KAYIT *isimg; 											// Kayit edilenleri alfabetik isim onceligine gore tutan isaretcim.
KAYIT *soyisimg;										// Kayit edilenleri alfabetik soyisim onceligine gore tutan isaretcim.

/* TODO (#1#): Programda kullanilan fonksiyon tanimlamalari */
void ada_gore_kayitlar(void);
void soyada_gore_kayitlar(void);
void rehber_dizisini_doldur(void);
int giris_ekrani(void);
void soyisme_gore_kayit(KAYIT *);
void isme_gore_kayit(KAYIT *);
void yeni_kayit_ekle(void);
void kayit_ara(void);
void devam_et(void);
void sirali_isim_sil(char []);
void sirali_soyisim_sil(char []);
void kayit_sil(void);
void rehberi_yazdir(void);

/* TODO (#2#): Main Fonksiyonu */
void main(void){
	int islem;
	rehber_dizisini_doldur();   						// Rehber dizisinin dolduruldugu alan.
	while(1){
		islem=giris_ekrani(); 							// Menu secme ekranini basar, girilen sayiyi returnlar.
		switch(islem){		  							// Girilen sayiya gore islem baslatilir
			case 1:
				yeni_kayit_ekle();						// Yeni Kayit Ekleme
				break;
			case 2:
				kayit_ara();							// Kayitli Ismi Arama
				break;
			case 3:
				kayit_sil();							// Kayitli Ismi Silme
				break;
			case 4:
				ada_gore_kayitlar();					// Kayitlari Isme Gore Siralar (Bubble Sort)
				break;
			case 5:
				soyada_gore_kayitlar();					// Kayitlari Soyisme Gore Siralar (Bubble Sort)
				break;
			case 6:
				rehberi_yazdir();						// Tüm Rehberi Yazdirma
				break;
			case 7:
				puts("Program sonlandirildi...");
				exit(1);     							// Programi Sonlandirir.
				break;
			default:
				printf("\n[HATA]: Hatali giris yapildi.\n");
				break;
		}	
		devam_et();	
	}	
}

void devam_et(void){
	fflush(stdin);
	printf("\nDevam etmek icin bir tusa basin.\n");
	getch();
}

/* TODO (#3#): Rehber dizisinin icini doldurma. */
void rehber_dizisini_doldur(){
	int i=0;
	int c='a';
	for(c='a' ; c<='z' ; c++ , i++){
		rehber[i].ilkHarf=c;
		rehber[i].ilkKayitPtr=NULL;
	}	
}

/* TODO (#4#): Giris - Secme ekrani */
int giris_ekrani(void){
	int girilen;
	puts("\n********** TELEFON REHBERI MENU **********");
	printf("[1] Yeni Kayit Ekle\n[2] Kayit Ara (Ad ve Soyad ile)\n[3] Kayit Sil (Ad ve Soyad ile)\n[4] Kayitlari Ada Gore Sirala\n[5] Kayitlari Soyada Gore Sirala\n[6] Tum Rehberi Yazdir\n[7] Cikis");
	printf("\n\nIslemi gir: ");	
	scanf("%d",&girilen);
	if (girilen>9){												// Girilen deger harf oldugunda programdan cikmak icin 7 returnlar.
		printf("\n[HATA]: Isleme HARF girdiniz.\n");			// Harflerin ASCII tablosunda baslangic degeri 60+ lerdedir.
		return 7;
	}
	return girilen;
}

/* TODO (#5#): Yeni Kayit Ekle */
void yeni_kayit_ekle(){
	fflush(stdin);
	int durum;
	KAYIT *kayitP , *ismeGore , *soyismeGore;					// Yeni kayit icin yeni struct ornekliyorum.
	
	kayitP = (KAYIT*) malloc(sizeof(KAYIT));					// Bellekten yeni kayit icin alan ayirdik.
	ismeGore = (KAYIT*) malloc(sizeof(KAYIT));					// Bellekten isme gore siralamak icin alan ayirdik.
	soyismeGore = (KAYIT*) malloc(sizeof(KAYIT));				// Bellekten soyisme gore siralamak icin alan ayirdik.
		
	printf("\nEklenecek Isim    : ");										// Kayit icin bilgileri aliyoruz
	scanf("%s",kayitP->ad);
	ismeGore->ad[50] = strcpy(ismeGore->ad,kayitP->ad);						// Girilen ad katarinin bellek adresini ismeGore->ad ina kopyaladim.
	soyismeGore->ad[50] = strcpy(soyismeGore->ad,kayitP->ad);
	
	printf("Eklenecek Soyisim : ");
	scanf("%s",kayitP->soyad);
	soyismeGore->soyad[50] = strcpy(soyismeGore->soyad,kayitP->soyad);		// Girilen soyad katarinin bellek adresini soyismeGore->soyad ina kopyaladim.
	ismeGore->soyad[50] = strcpy(ismeGore->soyad,kayitP->soyad);
	
	printf("Eklenecek Telefon : ");
	scanf("%d",&kayitP->telNo);	
	ismeGore->telNo = kayitP->telNo;
	soyismeGore->telNo = kayitP->telNo;
	
	durum=kayit_tamamla(kayitP);								// Aldigimiz bilgileri bagli listeye kayit ettiriyoruz.
	isme_gore_kayit(ismeGore);									// Isme gore kayit tamamlayan fonksiyon.
	soyisme_gore_kayit(soyismeGore);							// Soyisme gore kayit tamamlayan fonksiyon.
	
	//(durum!=0)?(printf("Kayit Tamamlandi\n")) : (printf("Ismi kucuk harflerle yaziniz\n"));
	(durum!=0)?( (durum==1)?(printf("\n")):(printf(" ve bu harfdeki ilk kayit.\n")) ) : (printf("[HATA]: Kayit Basarisiz. Turkce-Ozel harfler kullanmayiniz.\n"));
}

// ISME GORE KAYIT GERCEKLESTIREN FONKSIYON
void isme_gore_kayit(KAYIT *p){	
	KAYIT *a;
	a = isimg;
	if(isimg != NULL){											// isimg yapisinda daha onceden kayit varsa
		while(a->sonrakiKayit != NULL){							// en son kaydi bulup, yeni kaydimizin adresini gostermesini sagliyoruz.
			a = a->sonrakiKayit;
		}
		a->sonrakiKayit = p;
		a->sonrakiKayit->sonrakiKayit=NULL;
		//printf("%s %s KAYIT EDILDI\n",a->ad,a->soyad);
	}
	else{														// isimg yapisina ilk kayit ekleniyorsa
		isimg = (KAYIT*) malloc(sizeof(KAYIT));					// bellekten yer acmamiz gereklidir.
		isimg = p;	 											
		isimg->sonrakiKayit = NULL;
	}
}

// SOYISME GORE KAYIT GERCEKLESTIREN FONKSIYON
void soyisme_gore_kayit(KAYIT *p){
	KAYIT *a;
	a = soyisimg;
	if(soyisimg!=NULL){											// soyisimg yapisinda daha onceden kayit varsa.
		while(a->sonrakiKayit != NULL){							// en son kaydi bulup, yeni kaydimizin adresini gostermesini sagliyoruz.
			a = a->sonrakiKayit;
		}
		a->sonrakiKayit =p;
		a->sonrakiKayit->sonrakiKayit = NULL;
	}
	else{														// soyisimg yapisina ilk kayit ekleniyorsa
		soyisimg = (KAYIT*) malloc(sizeof(KAYIT));				// bellekten yer acmamiz gereklidir.
		soyisimg = p;
		soyisimg->sonrakiKayit = NULL;
	}
}

// NORMAL KAYDI TAMAMLAYAN FONKSIYON
int kayit_tamamla(KAYIT *p){
	int i;
	KAYIT *a;
	char harf = p->ad[0];										// Girilen ismin ilk harfline gore Rehber dizisine kayit edilecek.
	harf = tolower(harf);										// Bu kisimda harf degiskenini kucuk harfe ceviriyorum,
																// cunku buyuk harf girilse bile, kayit isleminin gerceklesmesini istiyorum.
																
	for(i=0;i<26;i++){										
		if(harf==rehber[i].ilkHarf){							// harf degiskeni rehber[] de tanimli olup olmadigina bakar.
			printf("[BILGI]: Kayit Basarili");
			if(rehber[i].ilkKayitPtr==NULL){  					// O harfde daha oce kayit yoksa.
	
				rehber[i].ilkKayitPtr=p;
				rehber[i].ilkKayitPtr->sonrakiKayit='\0';
				return 2;
			}
			else{ 							  					// O harfde daha onceden de kayit varsa.
				a=rehber[i].ilkKayitPtr;
				while(a->sonrakiKayit){
					a=a->sonrakiKayit;
				}
				a->sonrakiKayit=p;
				a->sonrakiKayit->sonrakiKayit='\0';
				return 1;
			}
		}
	}
	//free(p);													// Daha onceden ayrilan alani serbest birakir.
	return 0;
}

/* TODO (#6#): Kayit Ara (Ad ve Soyada göre) */
void kayit_ara(){
	fflush(stdin);
	int i,kontrol=0;
	char ad[50],soyisim[50];
	
	printf("Aranacak Isim    : ");
	scanf("%s",ad);
	printf("Aranacak Soyisim : ");
	scanf("%s",soyisim);
	
	KAYIT *b;
	for(i=0;i<26;i++){
		if(rehber[i].ilkKayitPtr!=NULL){
			b=rehber[i].ilkKayitPtr;
			while(b){
				if(!strcmp(b->ad,ad) && !strcmp(b->soyad,soyisim)){
					kontrol = 1;									// Kontrol degiskeniyle bulunup bulunmadigini belirliyoruz. 1 ise BULUNDU.
					printf("\nKayit Bulundu\n");
					printf("[TELEFON]: %d\n",b->telNo);
				}
				b=b->sonrakiKayit;			
			}
		}	
	}
	if(kontrol==0){
		printf("\n[BILGI]: Aradiginiz kayit mevcut degil.\n");
	}
}

/* TODO (#7#): Kayit Sil (Ad ve Soyada Gore) */
void kayit_sil(){
	KAYIT *once, *c;
	int i=0 , kontrol=0;
	char isim[50],soyisim[50];
	printf("\nSilinecek Isim    : ");
	scanf("%s",isim);
	printf("Silinecek Soyisim : ");
	scanf("%s",soyisim);
	while(i<26){
		if(rehber[i].ilkHarf==isim[0]){
			break;
		}
		i++;	
	}
	c = rehber[i].ilkKayitPtr;
	once = c;
	if(!strcmp(c->ad,isim) && !strcmp(c->soyad,soyisim)){			// Rehberin ilk isaret ettigi alan silinecekse (ilk kayit)
		rehber[i].ilkKayitPtr=c->sonrakiKayit;
		kontrol = 1;
		
		free(c);
	}
	else{
		c=c->sonrakiKayit;											// Ilk kayit haricinde bir kayit siliniyorsa.
		while(c != NULL){
		
			if(!strcmp(c->ad,isim) && !strcmp(c->soyad,soyisim)){
				// Burada ki c, silinecek eleman. 
				// Bir önceki elemanýn sonrakiKayit ini c->sonraki kayit la baðlamalýyýz.
				once->sonrakiKayit=c->sonrakiKayit;
				kontrol = 1;
				free(c);
				break;
			}
			once = c;
			c = c->sonrakiKayit;
		}
	}
	sirali_isim_sil(isim);
	sirali_soyisim_sil(soyisim);
	(kontrol==1)?(printf("\n[BILGI]: Kayit basarili bir sekilde silindi.\n")):(printf("[BILGI]: Kayit bulunamadigi icin silinemedi."));
}

// isimg isaretcisinden silen fonksiyon
void sirali_isim_sil(char isim[50]){
	KAYIT *p , *a;
	p = isimg;
	if(!strcmp(isim,isimg->ad)){									// isimg nin ilk elemaný silecekse.
		isimg = isimg->sonrakiKayit;
		free(p);
	}
	else{															// isimg nin ilk haric herhangi bir elemani silinecekse.
		while(p != NULL){
			if(!strcmp(isim,p->sonrakiKayit->ad)){
				a = p->sonrakiKayit;
				p->sonrakiKayit = p->sonrakiKayit->sonrakiKayit;
				free(a);
				break;
			}
			p = p->sonrakiKayit;
		}
	}
}

// soyisimg isaretcisinden silen fonksiyon
void sirali_soyisim_sil(char soyisim[50]){
	KAYIT *p , *a;
	p = soyisimg;
	if(!strcmp(soyisim,soyisimg->soyad)){							// soyisimg nin ilk elemaný silecekse.
		soyisimg = soyisimg->sonrakiKayit;
		free(p);
	}
	else{															// soyisimg nin ilk haric herhangi bir elemani silinecekse.
		while(p != NULL){
			if(!strcmp(soyisim,p->sonrakiKayit->soyad)){
				a = p->sonrakiKayit;
				p->sonrakiKayit = p->sonrakiKayit->sonrakiKayit;
				free(a);
				break;
			}
			p = p->sonrakiKayit;
		}
	}
}

/* TODO (#9#): Isme Gore Alfabetik Sirala */
void ada_gore_kayitlar(){
	int i=0;
	KAYIT *p;
	p=isimg;
	while(p != NULL){												// Bu döngü, kac adet kayit oldugunu belirler.
		i++;														// Bubble sort icin eleman sayisi gereklidir.
		p = p->sonrakiKayit;
	}
	ad_bubble_sort(i);												// Buradan isme gore siralama gerceklesir ve ekrana basilir.
	printf("\n[BILGI]: %d Kayit Isme Gore Siralandi.\n",i);	
}

/* TODO (#10#): Soyada Gore Alfabetik Sirala */ 
void soyada_gore_kayitlar(){
	int i=0;
	KAYIT *p;
	p = soyisimg;
	while(p != NULL){												// Bu döngü, kac adet kayit oldugunu belirler.
		i++;														// Bubble sort icin eleman sayisi gereklidir.
		p = p->sonrakiKayit;
	}
	soyad_bubble_sort(i);											// Buradan soyisme gore siralama gerceklesir ve ekrana basilir.
	printf("\n[BILGI]: %d Kayit Soyisme Gore Siralandi.\n",i);
}

// Isimleri Siralayan Asil Fonksiyon
void ad_bubble_sort(int n){
	KAYIT *p , *liste;
	char degis[50]="StringDegistiren";
	char degisS[50]="StringDegistiren";
	int degis_tel = 123456;
	int i,j;
	p=isimg;
	liste=isimg->sonrakiKayit;
	//printf("%d N DIR\n",n);
	for(i=n-1 ; i>0  ; i--){
		p = isimg;
		for(j=0 ; j<i ; j++){
			if(strcmp(p->ad,p->sonrakiKayit->ad)>0){
				/* ISIM DEGISTIRIR */
				strcpy(degis,p->ad);
				strcpy(p->ad,p->sonrakiKayit->ad);
				strcpy(p->sonrakiKayit->ad,degis);
				
				/* SOYISIM DEGISTIRIR */
				strcpy(degisS , p->soyad);
				strcpy(p->soyad , p->sonrakiKayit->soyad);
				strcpy(p->sonrakiKayit->soyad , degisS);
				
				// TELEFON DEGISTIRIR 
				degis_tel = p->telNo;
				p->telNo = p->sonrakiKayit->telNo;
				p->sonrakiKayit->telNo = degis_tel;		
			}
			p = p->sonrakiKayit;
		}
	}
	printf("\n\t[ISIM]\t\t[SOYISIM]\t\t[NUMARA]\n");
	printf("\t %-15s %-15s         %-15d\n",isimg->ad , isimg->soyad , isimg->telNo);
	//printf("%s\n",isimg->soyad);
	while(liste!=NULL){
		printf("\t %-15s %-15s         %-15d\n",liste->ad , liste->soyad , liste->telNo);
		//printf("%s\n",liste->soyad);
		liste= liste->sonrakiKayit;
	}
}

// Soyisimleri Siralayan Asil Fonksiyon
void soyad_bubble_sort(int n){
	KAYIT *p , *liste;
	char degis[50]="StringDegistiren";
	char degisS[50]="StringDegistiren";
	int degis_tel = 123456;
	int i,j;
	p=soyisimg;
	liste=soyisimg->sonrakiKayit;
	//printf("%d N DIR\n",n);
	for(i=n-1 ; i>0  ; i--){
		p = soyisimg;
		for(j=0 ; j<i ; j++){
			if(strcmp(p->soyad,p->sonrakiKayit->soyad)>0){
				/* ISIM DEGISTIRIR */
				strcpy(degis , p->ad);
				strcpy(p->ad , p->sonrakiKayit->ad);
				strcpy(p->sonrakiKayit->ad , degis);
				
				/* SOYISIM DEGISTIRIR */
				//degisS[50] = (char*) malloc(sizeof(char)*strlen(*(p)->soyad)); ->->->->->->->->  BU IFADEYI KULLANMAMIZIN SEBEBI;
				strcpy(degisS,p->soyad);														// ZATEN ATAMALARIN 50Byte LIK BÝR YER KAPLAMASIDIR. isim[50] den yani.
				strcpy(p->soyad,p->sonrakiKayit->soyad);										// ATAMALAR DIZI YERINE ISARETCILERE YAPILSAYDI,
				strcpy(p->sonrakiKayit->soyad,degisS);											// DEGERI DEGISECEK ISARETCI ICIN, ATANACAK ISARETCI
																								// BOYUTUNDA YER AYRILMASI GEREKECEKTI.
				// TELEFON DEGISTIRIR 
				degis_tel = p->telNo;
				p->telNo = p->sonrakiKayit->telNo;
				p->sonrakiKayit->telNo = degis_tel;		
			}
			p = p->sonrakiKayit;
		}
	}

	printf("\n\t[ISIM]\t\t[SOYISIM]\t\t[NUMARA]\n");
	printf("\t %-15s %-15s         %-15d\n",soyisimg->ad,soyisimg->soyad,soyisimg->telNo);
	//printf("%s\n",soyisimg->soyad);
	while(liste!=NULL){
		printf("\t %-15s %-15s         %-15d\n",liste->ad,liste->soyad,liste->telNo);
		//printf("%s\n",liste->soyad);
		liste= liste->sonrakiKayit;
	}
}

/* TODO (#10#): Rehberi Yazdir */
void rehberi_yazdir(void){
	int i,kontrol=0,toplam=0;
	KAYIT *p;
	for(i=0;i<26;i++){
		if(rehber[i].ilkKayitPtr!=NULL){
			kontrol=1;
			p=rehber[i].ilkKayitPtr;
			printf("\n[HARF %c]\n",rehber[i].ilkHarf);
			while(p){
				printf("\tIsim: %-15s Soyisim: %-15s telefon: %d\n",p->ad,p->soyad,p->telNo);
				p=p->sonrakiKayit;
				toplam++;
			}
		}	
	}
	if(kontrol==0){
		printf("\n[BILGI]: Su an rehberde hic kayit yok.\n");
	}
	if(toplam>0){
		printf("\n[BILGI]: %d Kisi Kayitli.",toplam);
	}
}



