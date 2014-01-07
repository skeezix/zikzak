/* menu eplorer par E.D Prod 1998
   etiennedeb@ifrance.com
   http://javascript.lab.cc
*/

//
//
// Rendez vous a la fin du script pour la création du menu
//
var clevel=1;
var type;type = new Array();
var level;level = new Array();
var texte;texte = new Array();
var status;status = new Array();
var image;image = new Array();
var lien;lien = new Array();
var pere;pere = new Array();
var ok=0;var nb=1;var numpere=0;var Apere=0;var Alevel=1;var oldpere;oldpere = new Array();
var major = parseInt(navigator.appVersion);
var agent = navigator.userAgent.toLowerCase();
var ns = ((agent.indexOf('mozilla')!=-1) && ((agent.indexOf('spoofer')==-1) && (agent.indexOf('compatible') == -1)));
var ns4 = (ns && (major == 4));
//
// la cible par default, ici c'est la fenetre ou cadre courant
// si vous visez un cadre appelle 'cadre1' par exmple, la variable doit etre egale a ' top.cadre1 '
//
var Acible=" top.main ";
var temp=0;
var ex_posx=8; // position absolue en x du menu
var ex_posy=224; // position absolue en y du menu

function ajouter(Anew,Atype,Atexte,Aimage,Alien) {
  if (Anew<0) {
  	numpere+=Anew;
  	if (numpere!=0) {
  	  Apere=oldpere[numpere];
 	    Alevel=level[Apere]+1;
  	}
  	else {
  	 Apere=0;numpere=0;Alevel=1;
  	}
  }
  if (Anew==1) {
	 Apere=0;numpere=0;Alevel=1;
  }
  type[nb]=Atype;
  level[nb]=Alevel;
  pere[nb]=Apere;
  status[nb]=0;
  texte[nb]=Atexte;
  image[nb]=Aimage;
  lien[nb]=Alien; 
  if (Atype==1) {oldpere[numpere]=Apere;Apere=nb;numpere+=1;Alevel+=1;}	
  nb+=1;
}

function verif(num){
 if (pere[num]!=0) {
  while (pere[num]!=0) {
   num=pere[num];
   if (status[num]==0) {ok=0;}
  }
 }
}

function excliquer(i) {
 if (lien[i]!="#") {
  chaine=Acible+".location.href='"+lien[i]+"';"
  eval(chaine);
 }
}

function AffExploMenu(){
 var y=0;
 for (var i = 1; i < texte.length; i++) {
 	CacherObj("explodiv"+i);
 }
 for (var i = 1; i < texte.length; i++) {
  ok=1;var nom="explodiv"+i;
  verif(i);
  if (ok==1) {
		PlacerObj(nom,ex_posx,ex_posy+y);VoirObj(nom);
		y+=16;
  }
 }
}

function bascule(numero) {
 status[numero]=1-status[numero];
 AffExploMenu();
}

function dessus(num) {
 if (!ns) {
  num.style.background="#707098";
  num.style.cursor="hand";
 }
}
function dehors(num) {
 if (!ns) {
  num.style.background="#505078";
  num.style.cursor="default";
 }
}

function CreerMenu(){
 var chaine = "";var saute=0;var niveau=1;var counter=0;
 for (var i = 1; i < texte.length; i++) {
   decalage="";special="";
   for (var j = 1; j < level[i]; j++) {
    if (j == level[i]-1) {
     decalage+="<img src='pictures/espacef.gif' align='top'>";}
    else {
     decalage+="<img src='pictures/espace.gif' align='top'>";}
   }
   if (type[i]==1) {
   	 special="onclick='bascule("+i+");return(false);' onmouseover='dessus(this);' onmouseout='dehors(this);'";
     if (ns4) {chaine="<layer name='explodiv"+i+"' width=300 height=16><nobr>"+decalage+"<font color='#CCCC00' face='verdana' size=2 ><a href='#' "+special+"><img src='pictures/dossier.gif' border='0' align='top'>&nbsp;"+texte[i]+"</font></A></nobr></layer>";}
     else{chaine="<div id='explodiv"+i+"' class='explodiv' "+special+"><nobr>"+decalage+"<img src='pictures/dossier.gif' border='0' align='top'>&nbsp;<font color='#FFFF80'>"+texte[i]+"</font></nobr></div>";}
   }
   else if (type[i]==2){
   	special="onclick='excliquer("+i+");return(false);' onmouseover='dessus(this);' onmouseout='dehors(this);'";
   	if (ns4) {chaine="<layer name='explodiv"+i+"' width=300 height=16 "+special+"><nobr>"+decalage+"<font face='verdana' size=2><a href='#' "+special+">"}
   	else {chaine="<div id='explodiv"+i+"' class='explodiv' "+special+"><nobr>"+decalage}
   	if (image[i]=="") {
      chaine+="<img src='pictures/lien.gif' border='0' align='top'>&nbsp;"}
    else {
    	chaine+="<img src='"+image[i]+"' border='0' align='top'>&nbsp;"
    }
    if (ns4) {chaine+=texte[i]+"</font></A></nobr></layer>";}
    else {chaine+=texte[i]+"</nobr></div>";}
   }
   document.write(chaine);
 }
}

// *****************************************************************
// Les entrees du menu sont crees ici
//
// argument 1: si mis a 1 : retour a la racine du menu
//             si mis a -x : remonter de x branches
//	       si mis a 0 : aucun changement d'arborescence
// argument 2: type de l'entree, 1 pour un dossier, 2 pour un lien
// argument 3: texte qui apparaitra dans le menu
// argument 4: Icone associe (laisser vide si pas d'icone)
// argument 5: Lien associe
//

//ajouter(1,1,"Dossier1","","#");
// ajouter(0,1,"Dossier2","","#");
//  ajouter(0,1,"Dossier3","","#");
//   ajouter(0,1,"Dossier4","","#");
//    ajouter(0,1,"Dossier5","","#");
//  ajouter(-3,1,"Dossier6","","#");
//   ajouter(0,2,"lien","zip.gif",'#'); 
//ajouter(1,2,"lien","zip.gif","#");
//ajouter(0,2,"lien","zip.gif",'#');

ajouter(1,2,"Introduction","","mainpage.html");
ajouter(0,2,"News","","news.html");
ajouter(0,1,"Articles","","#");
ajouter(0,2,"SoundServer","","articles/soundserver/article1.html");
ajouter(0,2,"DirectX SoundServer","","articles/soundserver/article2.html");
ajouter(0,2,"Emulator programming","","articles/pcteam/pcteam.html");
ajouter(0,2,"Three articles!","","articles/pcteam/pcteam2.html");
//ajouter(-1,1,"Professional","","#");
ajouter(1,2,"C.V and Games","","procv.html");
//ajouter(1,1,"Personal Projects","","#");
ajouter(1,1,"PC-Windows","","#");
ajouter(0,2,"DirectX Demos","","demos/demos.html");
ajouter(0,2,"RickDangerous 32Kb","","rick32/rick32.html");
ajouter(0,2,"LS4 Sound Synthesys","","ls4/ls4.html");
ajouter(1,1,"SainT (Emulator)","","");
ajouter(0,2,"Download","","SainT/saint.html");
ajouter(0,2,"Documentation","","SainT/saintdoc.html");
ajouter(0,2,"Screenshot","","SainT/screenshot.html");
ajouter(1,1,"ST-Sound","","");
ajouter(0,2,"What's that ?","","stsound.html");
ajouter(0,2,"Cardware Registration","","cardware.html");
ajouter(0,2,"NEW: OpenSource!","","StSoundGPL.html");
ajouter(0,2,"Download","","stsound_download.html");
ajouter(0,2,"YM File format","","ymformat.html");
ajouter(1,1,"GameBoy Advance","","#");
ajouter(0,2,"3D-Trip Demo","","GBA/stniccc/gba.html");
ajouter(0,2,"STSound Advance","","GBA/stsa/stsa.html");
ajouter(1,1,"ATARI","","#");
ajouter(0,2,"Demos I made","","ataridemos.html");
ajouter(0,2,"ST-NICCC","","stniccc/stniccc.html");
ajouter(0,2,"PC Dev-Kit","","stkit.html");
ajouter(0,2,"ATARI Record History","","record16/record16.html");
ajouter(0,2,"ATARI Record Contest !","","record16/sprite_contest.html");
ajouter(1,2,"Books Review","","booksreview.html");
ajouter(0,2,"Guest Book","","http://perso0.free.fr/cgi-bin/guestbook.pl?login=leonard.oxg");
ajouter(0,2,"Fave Links","","links.html");

// construction du menu, rien a changer ici
CreerMenu();