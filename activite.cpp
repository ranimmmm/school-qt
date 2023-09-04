#include "activite.h"

Activite::Activite()
{

}

int Activite::getId() const
{
    return id;
}

void Activite::setId(int value)
{
    id = value;
}

QString Activite::getMatricule() const
{
    return matricule;
}

void Activite::setMatricule(const QString &value)
{
    matricule = value;
}

QString Activite::getTitre() const
{
    return titre;
}

void Activite::setTitre(const QString &value)
{
    titre = value;
}

QString Activite::getType() const
{
    return type;
}

void Activite::setType(const QString &value)
{
    type = value;
}

QDate Activite::getDate_act() const
{
    return date_act;
}

void Activite::setDate_act(const QDate &value)
{
    date_act = value;
}

int Activite::getArchive() const
{
    return archive;
}

void Activite::setArchive(int value)
{
    archive = value;
}


bool Activite::ajouter(){
    QSqlQuery query;

    query.prepare("Insert Into ACTIVITE(MATRICULE,TITRE,TYPE,DATE_ACTIVITE,ARCHIVE) Values(:matricule,:titre,:type,:date,0);");
    query.bindValue(":matricule",this->matricule);
    query.bindValue(":titre",this->titre);
    query.bindValue(":type",this->type);
    query.bindValue(":date",this->date_act);


    return query.exec();
}

QSqlQueryModel * Activite::afficher(int archive){
    QSqlQueryModel * modal=new QSqlQueryModel();
    modal->setQuery("SELECT MATRICULE,TITRE,TYPE,DATE_ACTIVITE FROM activite WHERE ARCHIVE="+QString::number(archive)+";");

    return modal;
}

bool Activite::modifier(QString matricule){
    QSqlQuery query;

    query.prepare("UPDATE activite SET MATRICULE = :matricule ,TITRE = :titre ,TYPE = :type ,DATE_ACTIVITE = :date WHERE MATRICULE = :mat") ;
    query.bindValue(":matricule",this->matricule);
    query.bindValue(":titre",this->titre);
    query.bindValue(":type",this->type);
    query.bindValue(":date",this->date_act);
    query.bindValue(":mat",matricule);

    return  query.exec();
}


bool Activite::supprimer(QString matricule){

    QSqlQuery query;

    query.prepare("DELETE FROM activite WHERE matricule=:matricule");
    query.bindValue(":matricule",matricule);

    return query.exec();
}

QBarSeries* Activite::statistique(){

    QSqlQuery q1,q2,q3,q4;
    qreal tot=0,c1=0,c2=0,c3=0;

    q1.prepare("SELECT * FROM activite");
    q1.exec();

    q2.prepare("SELECT * FROM activite WHERE type='sportif'");
    q2.exec();

    q3.prepare("SELECT * FROM activite WHERE type='artistique'");
    q3.exec();

    q4.prepare("SELECT * FROM activite WHERE type='cultural'");
    q4.exec();


    while (q1.next()){tot++;}
    while (q2.next()){c1++;}
    while (q3.next()){c2++;}
    while (q4.next()){c3++;}


    c1=c1/tot;
    c2=c2/tot;
    c3=c3/tot;

    QBarSet *set0 = new QBarSet("sportif");
    QBarSet *set1 = new QBarSet("artistique");
    QBarSet *set2 = new QBarSet("cultural");

    // Assign values for each bar
    *set0 << c1;
    *set1 << c2;
    *set2 << c3;

    QBarSeries *series = new QBarSeries();
    series->append(set0);
    series->append(set1);
    series->append(set2);

    return series;

}

QSqlQueryModel* Activite::tri(QString type,int archive){

    QSqlQueryModel * modal=new QSqlQueryModel();

    QString query;

    modal->setQuery("SELECT MATRICULE,TITRE,TYPE,DATE_ACTIVITE FROM activite WHERE ARCHIVE = "+QString::number(archive)+" ORDER BY "+type+";");

    return modal;

}

QSqlQueryModel* Activite::rechercher(QString toSearch,QString type,int archive){
    QSqlQueryModel * modal=new QSqlQueryModel();

    QString query;

    if(type == "titre") {
        query = "SELECT MATRICULE,TITRE,TYPE,DATE_ACTIVITE FROM activite WHERE ARCHIVE="+QString::number(archive)+" and titre LIKE '%"+toSearch+"%';";
     }
     else if(type == "type"){
        query = "SELECT MATRICULE,TITRE,TYPE,DATE_ACTIVITE FROM activite WHERE ARCHIVE="+QString::number(archive)+" and type LIKE '%"+toSearch+"%';";
     }
     else if(type =="matricule"){
        query = "SELECT MATRICULE,TITRE,TYPE,DATE_ACTIVITE FROM activite WHERE ARCHIVE="+QString::number(archive)+" and matricule LIKE '%"+toSearch+"%';";
     }

    modal->setQuery(query);

    return modal;
}

void Activite::exporterPDF(QString matricule,QString titre,QString type){
    QTextDocument document;

    document.setPlainText("relevé d'activité \nMatricule : "+matricule+"\nTitre : "+titre+"\nType : "+type+"\n");

    QPrinter printer;
    printer.setPrinterName("desiered printer name");
    QPrintDialog dialog(&printer);

    if (dialog.exec() == QDialog::Accepted) {
        document.print(&printer);
    }
}

QStringList Activite::notifcation()
{
    QSqlQuery s;

    QStringList queryResultArray;

    s.prepare("select titre from Activite WHERE TRUNC(date_activite) = TRUNC(SYSDATE)");
    if (s.exec()) {
        while (s.next()) {
            QString columnData = s.value(0).toString(); // Change 0 to the index of your column
            queryResultArray.append(columnData);
        }
    }

    return queryResultArray;
}

bool Activite::archiver(QString matricule,int archive){
    QSqlQuery query;

    query.prepare("UPDATE activite SET ARCHIVE="+ QString::number(archive)+" WHERE MATRICULE = :mat") ;
    query.bindValue(":mat",matricule);

    return  query.exec();
}
