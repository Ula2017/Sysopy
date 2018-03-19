#include <stdio.h>
#include<stdlib.h>
#include <time.h>
#include<sys/times.h>
#include "Zadanie1.h"

position * generateContact(list * list, char ** name, char ** surname, char ** email, char **birthdate, char **phone, char** address){
position *p =malloc(sizeof(position));
int num=(rand()%95);
p->address=address[num];
p->birthdate=birthdate[num];
p->name=name[num];
p->surname=surname[num];
p->email=email[num];
p->phone=phone[num];
return p;
}
//real time
double realtime(clock_t start, clock_t stop){
return (double)(stop-start)/(double)CLOCKS_PER_SEC;
}
double usertime(struct tms* start, struct tms* stop){
return (double)((stop->tms_utime)-(start->tms_utime))/(double)CLOCKS_PER_SEC;
}
double systime(struct tms* start, struct tms* stop){
return (double)(stop->tms_stime-start->tms_stime)/(double)CLOCKS_PER_SEC;
}


int main(void){
    srand(time(NULL));

    char * name[] ={"Fitzgerald","Kiayada","Holly","Kellie","Stacy","Wendy","Ayanna","Noelle","Gannon","Madonna","Priscilla","Jerome","Cullen","Hiroko","Dylan","Kristen","Nolan","Ayanna","Kibo","Leila","Tamara","Stephen","Knox","Brody","Dennis","Delilah","Mohammad","Ivy","Boris","Nell","Tashya","Tarik","Aristotle","Charles","Ingrid","Stella","Fleur","Lesley","Adele","Dahlia","Buffy","Hanna","Shafira","Maya","Hanae","Violet","Brianna","Tara","Leigh","Marsden","Ivan","Macaulay","Rama","Hamish","Orson","Shaine","Ahmed","Kiayada","Risa","Eve","Meredith","Adrian","Kirby","Iona","Zelda","Holly","Quinn","Ann","Raya","Zorita","Lillith","Hyacinth","Ashely","Willa","Nomlanga","Orli","Hannah","Andrew","Belle","Graiden","Kane","Aphrodite","Laura","Aquila","Keith","Cadman","Rhona","Cally","Sheila","Uta","McKenzie","Ezekiel","Iona","Yvonne","Stacey","Renee","Lilah","Brenden","Guinevere","Morgan"};
    char * surname[ ]={"Kim","Shields","Orr","Larson","Taylor","Hewitt","Whitney","Oneill","Gibbs","Mullins","Wells","Trujillo","Barnes","Rogers","Lara","Campbell","Ortega","Rowe","Travis","Mcbride","Knapp","Hammond","Coffey","Brennan","Conley","Kaufman","Alvarado","Davidson","Nielsen","Riddle","Mccarthy","Bauer","Mcmahon","Jordan","Dean","Maddox","Hale","Cantrell","Phelps","Leach","Marquez","Kennedy","Carter","Daniels","Ashley","Francis","Deleon","Cash","Frye","Conley","Osborne","Shaw","Copeland","Delgado","Lancaster","Aguirre","Merrill","Allison","Black","Barber","Molina","English","Phillips","Little","Langley","Morin","Barlow","Bond","Lambert","Whitehead","Ellison","Jacobson","Vance","Castillo","Murphy","Frye","Brooks","Heath","Yang","Fleming","Reilly","Best","Harrison","Alford","Durham","Floyd","Ashley","Parrish","Osborne","Mann","Hatfield","Walls","Combs","Richardson","Sims","Nichols","Cote","Gibson","Davenport","Kennedy"};
    char * birthdate[]={"05-09-91","22-04-92","03-03-99","02-04-94","15-08-94","01-10-97","06-08-92","03-10-94","16-09-90","11-01-97","01-07-89","30-06-95","30-10-91","07-01-99","12-08-88","15-11-93","19-02-92","25-06-90","25-07-85","20-06-98","25-08-98","24-10-84","15-05-90","04-01-85","28-11-86","27-11-85","10-06-85","10-04-97","26-10-92","03-08-94","12-02-97","05-05-96","05-03-89","17-11-98","30-03-85","11-08-98","28-12-94","22-01-95","03-01-94","10-11-98","18-11-84","07-10-96","03-06-96","30-06-92","11-06-97","06-08-94","20-01-91","25-05-97","19-03-94","28-12-93","04-03-91","06-04-88","20-06-84","14-12-85","16-06-97","26-11-90","28-08-98","28-11-96","24-03-95","06-04-97","01-09-93","24-01-95","04-01-97","06-04-96","13-02-89","21-08-87","01-03-93","28-10-92","03-07-84","29-05-93","01-08-94","28-07-90","24-01-96","13-12-92","20-02-98","22-03-90","21-01-92","26-10-89","02-11-91","29-01-97","12-07-97","10-08-95","07-02-97","04-03-88","13-04-94","15-03-89","20-03-92","07-07-95","19-11-91","03-03-89","29-07-89","06-05-86","16-08-84","09-02-89","05-04-86","26-05-85","01-07-96","28-10-94","27-01-91"  };
    char * phone[]={"187497571","658427391","268501187","756732989","475330410","965085421","550742830","830036980","532930374","923552577","335588732","641030157","711192875","317527824","655805532","756941747","281969104","077531251","409063642","096976456","862555451","719835975","904432939","541948973","729588729","950381364","747526477","794693710","159647360","322419548","016872820","293357872","622351525","844263889","687753980","428610510","526779581","376762037","342013597","288280438","682852603","468400090","794947789","873199042","918248199","852786132","823411186","065684981","579971773","118380201","550156922","973690964","766966523","593705383","659287401","719988951","295928476","774796890","721112414","200208784","904511588","437341669","292358215","053230921","370600980","269473532","646584773","066267642","703002347","063448306","431549626","676101021","720635237","808681117","075724914","277697049","797757509","628437370","685004542","750886038","531865185","236092189","501218015","336720283","466332101","393621643","383136308","452210411","995375003","887198824","652930998","338087857","443115831","639195410","668256088","683222754","058283563","289224322","863643786","920170821"};
    char *email[]={"urna.Nunc.quis@mattis.com","ultrices.iaculis.odio@varius.co.uk","elementum.dui@placeratvelitQuisque.net","Donec.at.arcu@tellussemmollis.ca","feugiat.metus.sit@vitae.com","enim@varius.net","viverra@diamnuncullamcorper.net","nunc.Quisque.ornare@ultricesVivamus.co.uk","consectetuer.euismod@metuseu.org","semper.pretium.neque@eget.co.uk","Donec.at@maurisaliquam.com","nulla.at.sem@arcueu.com","amet@non.ca","leo.elementum@quisdiam.org","enim@diamProindolor.ca","ante.bibendum.ullamcorper@varius.co.uk","et@metus.edu","mi.Aliquam@facilisisfacilisis.net","tortor.nibh@natoquepenatibus.net","ac.arcu.Nunc@pellentesqueeget.co.uk","ipsum.Suspendisse@eutempor.ca","id.magna@Suspendissealiquet.org","vitae.nibh.Donec@sempertellus.com","sit.amet@ullamcorpervelitin.ca","Morbi.non@ut.co.uk","arcu.Vestibulum.ante@sapiencursusin.net","velit.Sed.malesuada@sit.org","ipsum.sodales.purus@bibendumullamcorper.edu","metus.vitae@mi.ca","pharetra.felis@erat.edu","eu.ligula.Aenean@Praesentinterdum.edu","fermentum.vel@eget.net","eget@cursusdiam.edu","sapien.Aenean.massa@mattisornarelectus.org","Morbi.non.sapien@atarcu.edu","blandit@duiFusce.net","tempus.lorem.fringilla@Sed.org","risus@tristique.net","nisl.Nulla.eu@vitaerisus.org","eleifend.egestas.Sed@ametfaucibusut.ca","nonummy.ipsum.non@In.org","lobortis.ultrices.Vivamus@sagittisaugue.ca","ipsum.dolor.sit@consequatlectus.ca","et.commodo.at@nisi.co.uk","mi.pede@Namacnulla.edu","Etiam.laoreet@natoquepenatibus.edu","nulla.at.sem@mauris.ca","Mauris@eratVivamusnisi.ca","aliquet@Maurisutquam.org","feugiat.non.lobortis@Aenean.co.uk","felis.ullamcorper@Phasellusdolor.org","malesuada@dui.com","pede.malesuada.vel@massalobortisultrices.co.uk","nunc.est.mollis@Class.ca","risus.Donec@loremfringillaornare.ca","neque.Morbi@venenatis.org","eu@enimSuspendissealiquet.org","lacus@Proin.co.uk","et@rutrum.com","aliquet.vel@tortor.net","posuere.at.velit@arcu.net","auctor.ullamcorper.nisl@ultriciesornare.com","diam@risusNulla.net","et.arcu@Donecnibh.org","ultrices.iaculis.odio@tempusnon.net","Class.aptent.taciti@volutpatNulla.ca","Proin.dolor.Nulla@magna.net","dui.Fusce.aliquam@nonfeugiatnec.com","quam.dignissim.pharetra@ipsum.co.uk","ultrices.Duis.volutpat@varius.com","mi@orci.org","Etiam@liberoest.com","elit.sed.consequat@Crasconvallis.ca","dictum.eu.placerat@duiFuscediam.co.uk","sem.ut.dolor@nuncnullavulputate.edu","enim.diam@at.ca","lectus.justo.eu@elit.edu","at.pede@risusa.co.uk","fringilla@ut.edu","egestas.blandit@Suspendisse.com","luctus.Curabitur@loremtristique.com","sociis.natoque.penatibus@egetmagnaSuspendisse.co.uk","tempor.lorem@urnaNunc.ca","Morbi.metus.Vivamus@maurisipsum.ca","interdum.Curabitur@ultricies.edu","parturient.montes@magnaDuisdignissim.org","semper.dui.lectus@acmieleifend.org","mi.lorem@Nullamenim.com","est.ac@id.co.uk","ultrices.mauris@Praesent.com","lacus.varius@pedenecante.co.uk","montes.nascetur@antebibendumullamcorper.org","mus.Donec.dignissim@libero.org","facilisis@penatibuset.net","Suspendisse.dui.Fusce@ac.org","ut@nonummyFusce.org","Cum@lobortis.com","ridiculus.mus@a.org","eros@erosnec.edu","amet.faucibus.ut@arcu.com"};
    char *address[]={"234 Box 872, 8398 Tristique St.","234 Box 272, 9131 Mauris Rd.","234 Box 224, 6531 Ante, Avenue","Ap #101-7405 Non Street","2828 Purus Rd.","2256 Luctus Rd.","410-9782 Mauris Street","463-5109 Eu Avenue","234 Box 528, 9235 Non Ave","695-5439 Nisl. Avenue","234 Box 670, 9354 Dolor Ave","234 Box 120, 1023 Congue, Street","1277 Eleifend Rd.","Ap #357-7121 Hendrerit Rd.","1070 Urna. Avenue","Ap #972-3358 Augue, St.","920-8647 Purus. Av.","9760 Eu Street","162-2616 Nam Rd.","5923 Mi St.","234 Box 898, 1130 Luctus St.","7982 Ornare, Rd.","4703 Sit Street","754 Fusce Street","3232 Enim St.","860-1014 Elementum Avenue","234 Box 533, 3000 Iaculis Road","422-8593 Rutrum Avenue","744-4096 Erat Ave","211-347 Ac Ave","9444 Curabitur St.","4584 Massa. St.","234 Box 239, 858 Non, Rd.","234 Box 565, 7030 Erat, Road","234 Box 646, 4066 Consectetuer St.","234 Box 718, 8060 Lobortis St.","320-8146 Neque Avenue","453-4262 Rutrum, Avenue","Ap #384-4243 Aliquam, St.","234 Box 410, 2542 Mauris St.","318-4803 In Avenue","982-1552 Sollicitudin Ave","234 Box 241, 7631 Tellus Street","513-2943 Nec Avenue","234 Box 336, 6080 Scelerisque Avenue","3812 Orci, Avenue","Ap #867-7411 Erat Av.","234 Box 217, 141 Et Rd.","4584 Nibh. St.","Ap #382-2645 Donec Rd.","916 Purus Av.","Ap #558-4773 Ultricies St.","824 Ut St.","234 Box 421, 8868 Tincidunt Street","Ap #369-899 Vel Rd.","4186 A St.","234 Box 564, 8377 Ultricies Avenue","4879 Leo. Ave","691-6190 Amet Road","4296 Dignissim. Rd.","Ap #417-8069 Iaculis, Road","234 Box 140, 3259 Urna, St.","Ap #278-5273 Facilisis Road","234 Box 944, 7647 Aliquam Road","1272 Vivamus Ave","5905 Nulla Rd.","234 Box 189, 6565 Parturient Rd.","9402 Tellus St.","265-9816 Id Rd.","Ap #113-8166 Tincidunt Av.","792-2357 Et Rd.","1545 Mauris Road","234 Box 377, 6281 Tincidunt. Avenue","8489 Est Av.","9356 Orci Rd.","234 Box 467, 5248 Elit. St.","1979 Imperdiet Road","8463 Augue Rd.","5684 Risus, Road","Ap #644-9337 Vestibulum Street","Ap #425-7298 Massa Street","234 Box 429, 5326 Neque Street","3978 Sed Street","332-303 Diam St.","559-918 Odio. St.","Ap #773-5381 Erat. Av.","234 Box 760, 9056 Tortor Rd.","234 Box 319, 6696 Mus. Ave","1193 Neque St.","Ap #967-8732 Dui Avenue","Ap #162-6280 Pellentesque Rd.","Ap #288-1696 Elit Street","9351 Turpis. St.","234 Box 757, 766 Adipiscing Road","912-4648 Adipiscing Street","766-9371 Erat Street","Ap #237-9898 Non Road","Ap #379-3760 Sociis Av."};
    struct tms *start;
    struct tms *stop;

    start=malloc(sizeof(struct tms));
    stop=malloc(sizeof(struct tms));
    clock_t realstart;
    clock_t realstop;


    printf("Time of creating contact list: \n");
    times(start);
    realstart=clock();
    list *l=createContactList();
    times(stop);
    realstop=clock();
    printf("realtime: %.10f , usertime: %.10f, systime: %.10f \n",realtime(realstart,realstop),usertime(start,stop),systime(start,stop));

    printf("Time of creating contact tree\n");
    times(start);
    realstart=clock();
    tree *t=createContactTree();
    times(stop);
    realstop=clock();
    printf("realtime: %.10f , usertime: %.10f, systime: %.10f\n",realtime(realstart,realstop),usertime(start,stop),systime(start,stop));

    for(int i=0; i<1000; i++){
        position *q=generateContact(l,name,surname, email,birthdate, phone,address);
        addContact(l,q);
        addContactTree(t,t->root,q);
    }

    position *p= malloc(sizeof(position));
    p->address="Mitery 1 30-508 krakow";
    p->birthdate="02-07-1998";
    p->name="Jack";
    p->surname="Zzztack";
    p->email="jackstack@onet.pl";
    p->phone="123456789";
    addContact(l,p);
    addContactTree(t,t->root,p);

    position *q= malloc(sizeof(position));
    q->address="Mitery 1 30-508 krakow";
    q->birthdate="02-07-1998";
    q->name="Jack";
    q->surname="Aaaaack";
    q->email="jackstack@onet.pl";
    q->phone="123456789";
    addContact(l,q);

    printf("Time of adding contact to the tree:\n");
    times(start);
    realstart=clock();
    addContactPositionTree(t,"Ula","Sobon","fsfsd", "fsdfdfd", "765432567", "fsfsdfsdfds");
    times(stop);
    realstop=clock();
    printf("realtime: %.10f , usertime: %.10f, systime: %.10f \n",realtime(realstart,realstop),usertime(start,stop),systime(start,stop));

    printf("Time of adding contact to the list:\n");
    times(start);
    realstart=clock();
    addContactPosition(l, "Ula", "Dsds", "fsfds", "fsdfdfd", "765432567", "fsfsdfsdfds");
    times(stop);
    realstop=clock();
    printf("realtime: %.10f , usertime: %.10f, systime: %.10f \n",realtime(realstart,realstop),usertime(start,stop),systime(start,stop));

    tree *tr;
    printf(tr->root->left);


    printf("Time of removing contact from list pessimistic: \n");
    times(start);
    realstart=clock();
    deleteContact(l,p);
    times(stop);
    realstop=clock();
    printf("realtime: %.10f , usertime: %.10f, systime: %.10f \n",realtime(realstart,realstop),usertime(start,stop),systime(start,stop));

    printf("Time of removing contact from list optimisitc: \n");
    times(start);
    realstart=clock();
    deleteContact(l,q);
    times(stop);
    realstop=clock();
    printf("realtime: %.10f , usertime: %.10f, systime: %.10f \n",realtime(realstart,realstop),usertime(start,stop),systime(start,stop));

    treeNode *node=malloc(sizeof(treeNode));
    node->pos=p;
    node->left=NULL;
    node->right=NULL;
    node->parent=NULL;

    printf("Time of removing contact from tree: \n");
    times(start);
    realstart=clock();
    deleteContactT(t,node);
    times(stop);
    realstop=clock();
    printf("realtime: %.10f , usertime: %.10f, systime: %.10f \n",realtime(realstart,realstop),usertime(start,stop),systime(start,stop));


    printf("Time of finding contact in tree: \n");
    times(start);
    realstart=clock();
    findContactPhoneT(t->root,"123456789");
    times(stop);
    realstop=clock();
    printf("realtime: %.10f , usertime: %.10f, systime: %.10f \n",realtime(realstart,realstop),usertime(start,stop),systime(start,stop));

    printf("Time of sort contact in list by phone number: \n");
    times(start);
    realstart=clock();
    sortBy_(l,"phone");
    times(stop);
    realstop=clock();
    printf("realtime: %.10f , usertime: %.10f, systime: %.10f \n",realtime(realstart,realstop),usertime(start,stop),systime(start,stop));

   printf("Time of finding contact in list pessimistic: \n");
    times(start);
    realstart=clock();
    findContact(l,p);
    times(stop);
    realstop=clock();
    printf("realtime: %.10f , usertime: %.10f, systime: %.10f \n",realtime(realstart,realstop),usertime(start,stop),systime(start,stop));

    printf("Time of finding contact in list optimistic: \n");
    times(start);
    realstart=clock();
    findContact(l,q);
    times(stop);
    realstop=clock();
    printf("realtime: %.10f , usertime: %.10f, systime: %.10f \n",realtime(realstart,realstop),usertime(start,stop),systime(start,stop));

}
