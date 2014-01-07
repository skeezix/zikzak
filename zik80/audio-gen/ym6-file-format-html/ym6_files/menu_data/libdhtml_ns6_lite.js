<!--- hide script from old browsers
/* 
DHTML Librairie NS6.x (Version 2.3 allégée - 03/12/2000)
Copyright E.D Prod.
WEB: http://javascript-lab.nexen.net
Mail: javascript-lab@nexen.net
!!! Tous droits réservés. Aucune partie de ce script ne peut être modifié sans la permission préalable de l'auteur.
*/
var vers=parseFloat(navigator.appVersion);if (vers<4.0) {alert("AVERTISSEMENT: Ce script requiert un navigateur de 4ieme génération.");}
var MXP=65535;var MYP=65535;var CMDS=new Array();var ptcom=0;var attente=0;var nbptcom=0;var pt=0;var pt2=0;var pt3=0;
var BUF=new Array();var com="";var cx1=0;var cy1=0;var cx2=0;var cy2=0;var nbi=0;
var temp="";var nom="";var dx=0.1;var dy=0.1;var rx=0;var ry=0;var decaX=0;var decaY=0;coz=new Array();var zin=new Array();
var SobjX=new Array();var SobjY=new Array();var Nobj=new Array();var Dobj=new Array();var DDobj=new Array();var PtObj=0;
for (var i = 0; i < 360; i++) {zin[i]=Math.sin((2*Math.PI)*i/360);coz[i]=Math.cos((2*Math.PI)*i/360);}

function CreerObj(nom,px,py,tx,ty,visible,zindex,contenu,special,dragdrop){
	if (visible==1) {visi="visible;"} else {visi="hidden;"}
  chaine='<div style="position:absolute;width:'+tx+'px;height:'+ty+'px;top:'+py+'px;left:'+px+'px;visibility:'+visi+';z-index:'+zindex+';" ID="'+nom+'" '+special+'>'+contenu+'</div>';
  document.write(chaine);
}
function CreerZone(nom,x1,y1,x2,y2){document.getElementById(nom).style.clip = "rect("+y1+","+x2+","+y2+","+x1+")";}
function ChangeIndex(nom,valeur) {document.getElementById(nom).style.zIndex=valeur;}
function CacherObj(nom){document.getElementById(nom).style.visibility = "hidden";}
function VoirObj(nom){document.getElementById(nom).style.visibility = "visible";}
function ModifierObj(nom,contenu){document.getElementById(nom).innerHTML=contenu;}
function PlacerObj(nom,px,py) {if (px!=-10000) {document.getElementById(nom).style.left = px;} if (py!=-10000) {document.getElementById(nom).style.top = py;};}
function ObjX(nom){var chaine=document.getElementById(nom).style.left;var value=parseInt(chaine.substring(0,chaine.length-2));return value;}
function ObjY(nom){var chaine=document.getElementById(nom).style.top;var value=parseInt(chaine.substring(0,chaine.length-2));return value;}
function Mouvement(evnt) {MXP = evnt.clientX;MYP = evnt.clientY;return false;}
document.onmousemove = Mouvement;
function SourisX(){var value=MXP+window.pageXOffset;return value;}
function SourisY(){var value=MYP+window.pageYOffset;return value;}
function TailleX(){var value=window.innerWidth;return value;}
function TailleY(){var value=window.innerHeight;return value;}
function OffsetX(){var value=window.pageXOffset;return value;}
function OffsetY(){var value=window.pageYOffset;return value;}
// end hiding --->
