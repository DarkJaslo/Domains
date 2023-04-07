#include "Board.hh"

const char pi[]= "3141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844609550582231725359408128481117450284102701938521105559644622948954930381964428810975665933446128475648233786783165271201909145648566923460348610454326648213393607260249141273724587006606315588174881520920962829254091715364367892590360011330530548820466521384146951941511609433057270365759591953092186117381932611793105118548074462379962749567351885752724891227938183011949129833673362440656643086021394946395224737190702179860943702770539217176293176752384674818467669405132000568127145263560827785771342757789609173637178721468440901224953430146549585371050792279689258923542019956112129021960864034418159813629774771309960518707211349999998372978049951059731732816096318595024459455346908302642522308253344685035261931188171010003137838752886587533208381420617177669147303598253490428755468731159562863882353787593751957781857780532171226806613001927876611195909216420198938095257201065485863278865936153381827968230301952035301852968995773622599413891249721775283479131515574857242454150695950829533116861727855889075098381754637464939319255060400927701671139009848824012858361603563707660104710181942955596198946767837449448255379774726847104047534646208046684259069491293313677028989152104752162056966024058038150193511253382430035587640247496473263914199272604269922796782354781636009341721641219924586315030286182974555706749838505494588586926995690927210797509302955321165344987202755960236480665499119881834797753566369807426542527862551818417574672890977772793800081647060016145249192173217214772350141441973568548161361157352552133475741849468438523323907394143334547762416862518983569485562099219222184272550254256887671790494601653466804988627232791786085784383827967976681454100953883786360950680064225125205117392984896084128488626945604241965285022210661186306744278622039194945047123713786960956364371917287467764657573962413890865832645995813390478027590099465764078951269468398352595709825822620522489407726719478268482601476990902640136394437455305068203496252451749399651431429809190659250937221696461515709858387410597885959772975498930161753928468138268683868942774155991855925245953959431049972524680845987273644695848653836736222626099124608051243884390451244136549762780797715691435997700129616089441694868555848406353422072225828488648158456028506016842739452267467678895252138522549954666727823986456596116354886230577456498035593634568174324112515076069479451096596094025228879710893145669136867228748940560101503308617928680920874760917824938589009714909675985261365549781893129784821682998948722658804857564014270477555132379641451523746234364542858444795265867821051141354735739523113427166102135969536231442952484937187110145765403590279934403742007310578539062198387447808478489683321445713868751943506430218453191048481005370614680674919278191197939952061419663428754440643745123718192179998391015919561814675142691239748940907186494231961567945208095146550225231603881930142093762137855956638937787083039069792077346722182562599661501421503068038447734549202605414665925201497442850732518666002132434088190710486331734649651453905796268561005508106658796998163574736384052571459102897064140110971206280439039759515677157700420337869936007230558763176359421873125147120532928191826186125867321579198414848829164470609575270695722091756711672291098169091528017350671274858322287183520935396572512108357915136988209144421006751033467110314126711136990865851639831501970165151168517143765761835155650884909989859982387345528331635507647918535893226185489632132933089857064204675259070915481416549859461637180270981994309924488957571282890592323326097299712084433573265489382391193259746366730583604142813883032038249037589852437441702913276561809377344403070746921120191302033038019762110110044929321516084244485963766983895228684783123552658213144957685726243344189303968642624341077322697802807318915441101044682325271620105265227211166039666557309254711055785376346682065310989652691862056476931257058635662018558100729360659876486117910453348850346113657686753249441668039626579787718556084552965412665408530614344431858676975145661406800700237877659134401712749470420562230538994561314071127000407854733269939081454664645880797270826683063432858785698305235808933065757406795457163775254202114955761581400250126228594130216471550979259230990796547376125517656751357517829666454779174501129961489030463994713296210734043751895735961458901938971311179042978285647503203198691514028708085990480109412147221317947647772622414254854540332157185306142288137585043063321751829798662237172159160771669254748738986654949450114654062843366393790039769265672146385306736096571209180763832716641627488880078692560290228472104031721186082041900042296617119637792133757511495950156604963186294726547364252308177036751590673502350728354056704038674351362222477158915049530984448933309634087807693259939780541934144737744184263129860809988868741326047215695162396586457302163159819319516735381297416772947867242292465436680098067692823828068996400482435403701416314965897940924323789690706977942236250822168895738379862300159377647165122893578601588161755782973523344604281512627203734314653197777416031990665541876397929334419521541341899485444734567383162499341913181480927777103863877343177207545654532207770921201905166096280490926360197598828161332316663652861932668633606273567630354477628035045077723554710585954870279081435624014517180624643626794561275318134078330336254232783944975382437205835311477119926063813346776879695970309833913077109870408591337464144282277263465947047458784778720192771528073176790770715721344473060570073349243693113835049316312840425121925651798069411352801314701304781643788518529092854520116583934196562134914341595625865865570552690496520985803385072242648293972858478316305777756068887644624824685792603953527734803048029005876075825104747091643961362676044925627420420832085661190625454337213153595845068772460290161876679524061634252257719542916299193064553779914037340432875262888963995879475729174642635745525407909145135711136941091193932519107602082520261879853188770584297259167781314969900901921169717372784768472686084900337702424291651300500516832336435038951702989392233451722013812806965011784408745196012122859937162313017114448464090389064495444006198690754851602632750529834918740786680881833851022833450850486082503930213321971551843063545500766828294930413776552793975175461395398468339363830474611996653858153842056853386218672523340283087112328278921250771262946322956398989893582116745627010218356462201349671518819097303811980049734072396103685406643193950979019069963955245300545058068550195673022921913933918568034490398205955100226353536192041994745538593810234395544959778377902374216172711172364343543947822181852862408514006660443325888569867054315470696574745855033232334210730154594051655379068662733379958511562578432298827372319898757141595781119635833005940873068121602876496286744604774649159950549737425626901049037781986835938146574126804925648798556145372347867330390468838343634655379498641927056387293174872332083760112302991136793862708943879936201629515413371424892830722012690147546684765357616477379467520049075715552781965362132392640616013635815590742202020318727760527721900556148425551879253034351398442532234157623361064250639049750086562710953591946589751413103482276930624743536325691607815478181152843667957061108615331504452127473924544945423682886061340841486377670096120715124914043027253860764823634143346235189757664521641376796903149501910857598442391986291642193994907236234646844117394032659184044378051333894525742399508296591228508555821572503107125701266830240292952522011872676756220415420516184163484756516999811614101002996078386909291603028840026910414079288621507842451670908700069928212066041837180653556725253256753286129104248776182582976515795984703562226293486003415872298053498965022629174878820273420922224533985626476691490556284250391275771028402799806636582548892648802545661017296702664076559042909945681506526530537182941270336931378517860904070866711496558343434769338578171138645587367812301458768712660348913909562009939361031029161615288138437909904231747336394804575931493140529763475748119356709110137751721008031559024853090669203767192203322909433467685142214477379393751703443661991040337511173547191855046449026365512816228824462575916333039107225383742182140883508657391771509682887478265699599574490661758344137522397096834080053559849175417381883999446974867626551658276584835884531427756879002909517028352971634456212964043523117600665101241200659755851276178583829204197484423608007193045761893234922927965019875187212726750798125547095890455635792122103334669749923563025494780249011419521238281530911407907386025152274299581807247162591668545133312394804947079119153267343028244186041426363954800044800267049624820179289647669758318327131425170296923488962766844032326092752496035799646925650493681836090032380929345958897069536534940603402166544375589004563288225054525564056448246515187547119621844396582533754388569094113031509526179378002974120766514793942590298969594699556576121865619673378623625612521632086286922210327488921865436480229678070576561514463204692790682120738837781423356282360896320806822246801224826117718589638140918390367367222088832151375560037279839400415297002878307667094447456013455641725437090697939612257142989467154357846878861444581231459357198492252847160504922124247014121478057345510500801908699603302763478708108175450119307141223390866393833952942578690507643100638351983438934159613185434754649556978103829309716465143840700707360411237359984345225161050702705623526601276484830840761183013052793205427462865403603674532865105706587488225698157936789766974220575059683440869735020141020672358502007245225632651341055924019027421624843914035998953539459094407046912091409387001264560016237428802109276457931065792295524988727584610126483699989225690";
const int permutations[24][4] = {{0,1,2,3},{0,1,3,2},{0,2,1,3},{0,2,3,1},{0,3,1,2},{0,3,2,1},{1,0,2,3},{1,0,3,2},{1,2,0,3},{1,2,3,0},{1,3,0,2},{1,3,2,0},{2,0,1,3},{2,0,3,1},{2,1,0,3},{2,1,3,0},{2,3,0,1},{2,3,1,0},{3,0,1,2},{3,0,2,1},{3,1,0,2},{3,1,2,0},{3,2,0,1},{3,2,1,0}};
int jump;
int randind;
int randsize(){return 9973;}
int randjump(){return 11;}
int randdigit(){
  randind += jump;
  if(randind>=randsize()) randind%=randsize();
  char c = pi[randind];
  c-='0';
  return c;
}

int GameInfo::randomNumber(int l, int r){
  //cerr << "starting random number generation" << endl;
  int div = 100000;
  int digits = 5;  
  int number = 0;
  for(int i = digits; i > 0; --i){
    number*=10;
    number += randdigit();
  }
  int res = r-l+1;
  res *= number;
  res /= div;
  res += l;
  if(res > r)--res; //the kind of thing that actually doesn't happen, but happens
  return res;
}

vector<int> GameInfo::randomPermutation(){
  vector<int> v(4);
  int index = randomNumber(0,23);
  v[0] = permutations[index][0];
  v[1] = permutations[index][1];
  v[2] = permutations[index][2];
  v[3] = permutations[index][3];
  return v;
}

//Board class
Board::Board(){}

void Board::iniBoard(int s){
  seed = s;
  randind = seed%randsize();
  jump = seed%randjump()+1;   //+1 to avoid jump = 0
  info.readSettings();
  info.game_map = vector<vector<char>>(info.boardHeight,vector<char>(info.boardWidth,'.'));
  info.square_map = vector<vector<Square>>(info.boardHeight,vector<Square>(info.boardWidth));
  for(int i = 0; i < info.boardHeight; ++i){
    for(int j = 0; j < info.boardWidth; ++j){
      info.square_map[i][j].p = Position(i,j);
      info.square_map[i][j].uDrawer = -1;
      info.square_map[i][j].plPainter = -1;
      info.square_map[i][j].isBorder = false;
      info.square_map[i][j].plDrawer = -1;
      info.square_map[i][j].u = nullptr;
      info.square_map[i][j].b = nullptr;
      info.square_map[i][j].closes = false;
    }
  }

  info.unitsVector.push_back(Unit(0,0,Position(5,5)));

  info.square_map[5][5].isBorder = true;
  info.square_map[5][5].plPainter = 0;
  info.square_map[5][5].u = &info.unitsVector[0];
}

bool Board::unitOk(int uid)const {return uid >= 0 and uid < info.unitsVector.size();}

void Board::erasePath(int uid, Position p){
  cerr << "erasing path" << endl;
  if(info.posOk(p) and info.square_map[p.x][p.y].uDrawer == uid){
    info.square_map[p.x][p.y].uDrawer = -1;
    info.square_map[p.x][p.y].plDrawer = -1;
    Position pp;
    pp = p+Direction::up;
    erasePath(uid,pp);
    pp = p+Direction::down;
    erasePath(uid,pp);
    pp = p+Direction::left;
    erasePath(uid,pp);
    pp = p+Direction::right;
    erasePath(uid,pp);
  }
}

void Board::killUnit(Unit& u){
  u.pl = -1;
  info.square_map[u.p.x][u.p.y].u = nullptr;
  erasePath(u.id_,u.p);
  killedUnits[u.id_] = true;
}

void Board::deenclose(Position p){
  if(info.posOk(p)){
    if(info.square(p).closes){
      info.square_map[p.x][p.y].closes = false;
      Position pp = p+Direction::up;
      deenclose(pp);
      pp = p+Direction::down;
      deenclose(pp);
      pp = p+Direction::left;
      deenclose(pp);
      pp = p+Direction::right;
      deenclose(pp);
    }
  }
}

void Board::enclose(int plId, int uid, Position p, int& xmin, int& xmax, int& ymin, int& ymax){
  //cerr << "enclosing " << p.x << "," << p.y  << endl;
  if(info.posOk(p)){
    if(not info.square(p).closes and (info.square(p).border() or info.square(p).drawer() == uid)){
      info.square_map[p.x][p.y].closes = true;
      if(not info.square(p).border()){
        info.square_map[p.x][p.y].isBorder = true;
      }
      cerr << p.x << "," << p.y << " is border" << endl;
      if(p.x < xmin) xmin = p.x;
      if(p.x > xmax) xmax = p.x;
      if(p.y < ymin) ymin = p.y;
      if(p.y > ymax) ymax = p.y;
      Position pp;
      pp = p+Direction::up;
      enclose(plId,uid,pp,xmin,xmax,ymin,ymax);
      pp = p+Direction::down;
      enclose(plId,uid,pp,xmin,xmax,ymin,ymax);
      pp = p+Direction::left;
      enclose(plId,uid,pp,xmin,xmax,ymin,ymax);
      pp = p+Direction::right;
      enclose(plId,uid,pp,xmin,xmax,ymin,ymax);
    }
  }
}

//used to flood
#define COLORINDEX 10

void Board::flood(int plId, int col, Position p, bool& flooded, bool& ok, vector<vector<Square>>& grid){

  if(p.x >= 0 and p.y >= 0 and p.x < grid.size() and p.y < grid[0].size() and (not grid[p.x][p.y].border() or (grid[p.x][p.y].plPainter != plId and grid[p.x][p.y].drawer() != plId)) and grid[p.x][p.y].plPainter < COLORINDEX){

    //This region must not be painted
    if(p.x == 0 or p.y == 0 or p.x == grid.size()-1 or p.y == grid[0].size()-1){
      cerr << "don't need to flood here: ";
      if(ok) ok = false;
    }

    cerr << "flooding position " << p.x << "," << p.y << " with color " << col << endl;
    grid[p.x][p.y].plPainter = col;
    if(not flooded) flooded = true;
    Position pp;
    pp = p+Direction::up;
    flood(plId,col,pp,flooded,ok,grid);
    pp = p+Direction::down;
    flood(plId,col,pp,flooded,ok,grid);
    pp = p+Direction::left;
    flood(plId,col,pp,flooded,ok,grid);
    pp = p+Direction::right;
    flood(plId,col,pp,flooded,ok,grid);
  }
}

void Board::paint(int plId, int uid, Position p){
  cerr << "painting" << endl;
  int xmin,xmax,ymin,ymax;
  xmin = xmax = p.x;
  ymin = ymax = p.y;
  enclose(plId,uid,p,xmin,xmax,ymin,ymax);
  cerr << xmin << " " << xmax << " " << ymin << " " << ymax << endl;
  deenclose(p);

  vector<vector<Square>> box(xmax-xmin+1,vector<Square>(ymax-ymin+1));

  //Copy the container box
  for(int i = xmin; i <= xmax; ++i){
    for(int j = ymin; j <= ymax; ++j){
      box[i-xmin][j-ymin] = info.square_map[i][j];
    }
  }
  int colIndex = COLORINDEX;
  vector<int> colors; //stores all used colors
  bool flooded = false;
  bool correct = true;

  //Floods all positions and stores which colors flooded a closed zone
  for(int i = 1; i < box.size()-1; ++i){
    for(int j = 1; j < box[0].size()-1; ++j){
      if(box[i][j].plPainter != plId){
        flood(plId,colIndex,Position(i,j),flooded,correct,box);
        if(flooded){
          flooded = false;
          if(correct){
            cerr << "color " << colIndex << " flooded" << endl;
            colors.push_back(colIndex);
          }
          else correct = true;
          colIndex++;
        }
      }
    }
  }

  cerr << box.size() << " " << box[0].size() << endl;

  //Paints all correctly flooded zones.
  for(int i = 0; i < box.size(); ++i){
    for(int j = 0; j < box[0].size(); ++j){
      if(box[i][j].border() and box[i][j].plDrawer == plId){
        cerr << "painting position " << xmin+i << "," << ymin+j << endl;
        box[i][j].plPainter = plId;
        box[i][j].plDrawer = -1;
        box[i][j].uDrawer = -1;
        continue;
      }
      if(box[i][j].plPainter > 0){
        //cerr << "plpainter > 0" << endl;
        bool found = false;
        //cerr << "size: " << colors.size() << endl;
        int k = 0;
        while(not found and k < colors.size()){
          //cerr << colors[k] << " ";
          if(colors[k] == box[i][j].plPainter) found = true;
          ++k;
        }
        //cerr << endl;
        if(found){
          //cerr << "found valid color" << endl;
          box[i][j].plPainter = plId;
          cerr << "painting position " << xmin+i << "," << ymin+j << endl;
        }
        else if(box[i][j].plDrawer == plId){
          cerr << "painting position " << xmin+i << "," << ymin+j << endl;
          box[i][j].plPainter = plId;
          box[i][j].plDrawer = -1;
          box[i][j].uDrawer = -1;
        }
      }
    }
  }

  //Copies the box back to the main grid
  for(int i = xmin; i <= xmax; ++i){
    for(int j = ymin; j <= ymax; ++j){
      if(box[i-xmin][j-ymin].plPainter < COLORINDEX) info.square_map[i][j] = box[i-xmin][j-ymin];
    }
  }

  //Updates border and erases drawings
  cerr << "updating border and erasing drawings..." << endl;
  for(int i = xmin-1; i <= xmax+1; ++i){
    for(int j = ymin-1; j <= ymax+1; ++j){
      Position pos = Position(i,j);
      if(info.posOk(pos)){
        if(info.square_map[pos.x][pos.y].drawed() and i >= xmin and i <= xmax and info.square_map[pos.x][pos.y].plPainter == plId){
          cerr << "erasing drawing starting at " << i << "," << j << endl;
          erasePath(info.square_map[pos.x][pos.y].drawer(),pos);
        }
        if(info.square_map[pos.x][pos.y].plPainter == plId){
          bool border = false;
          for(int i = 0; i < 1; ++i){
            Position pos2 = pos+Direction::UL;
            if(info.posOk(pos2) and info.square_map[pos2.x][pos2.y].plPainter != plId){border = true;break;}
            pos2 = pos+Direction::up;
            if(info.posOk(pos2) and info.square_map[pos2.x][pos2.y].plPainter != plId){border = true;break;}
            pos2 = pos+Direction::UR;
            if(info.posOk(pos2) and info.square_map[pos2.x][pos2.y].plPainter != plId){border = true;break;}
            pos2 = pos+Direction::left;
            if(info.posOk(pos2) and info.square_map[pos2.x][pos2.y].plPainter != plId){border = true;break;}
            pos2 = pos+Direction::right;
            if(info.posOk(pos2) and info.square_map[pos2.x][pos2.y].plPainter != plId){border = true;break;}
            pos2 = pos+Direction::DL;
            if(info.posOk(pos2) and info.square_map[pos2.x][pos2.y].plPainter != plId){border = true;break;}
            pos2 = pos+Direction::down;
            if(info.posOk(pos2) and info.square_map[pos2.x][pos2.y].plPainter != plId){border = true;break;}
            pos2 = pos+Direction::DR;
            if(info.posOk(pos2) and info.square_map[pos2.x][pos2.y].plPainter != plId){border = true;break;}
          }
          
          if(border){
            if(not info.square_map[pos.x][pos.y].border()) info.square_map[pos.x][pos.y].isBorder = true;
          }
          else{
            if(info.square_map[pos.x][pos.y].border()) info.square_map[pos.x][pos.y].isBorder = false;
          }
        }
      }
    }
  }
  cerr << "paint func end " << endl;
}

void Board::draw(int plId, int uid, Position pnew, Position pant){
  cerr << "drawing" << endl;
  
  Square sant = info.square(pant);
  Square snew = info.square(pnew);
  
  
  if(snew.drawed() and snew.uDrawer != uid){
    //if you step on someone else's drawing, erase it
    erasePath(snew.uDrawer,pnew);
  }
  if(sant.uDrawer == uid){
    //draws if behind you is a drawing
    info.square_map[pnew.x][pnew.y].uDrawer = uid;
    info.square_map[pnew.x][pnew.y].plDrawer = plId;
    //if behind you is a drawing and you enter a secure painted area, enclose and paint
    if(snew.plPainter == plId){
      paint(plId, uid, sant.p);
    }
  }
  if(sant.plPainter == plId and snew.plPainter != plId){
    //draws if exiting a secure painted area
    info.square_map[pnew.x][pnew.y].uDrawer = uid;
    info.square_map[pnew.x][pnew.y].plDrawer = plId;
  }
}

int Board::fight(Unit& u1, Unit& u2){
  cerr << "fighting" << endl;
  //Transformations to energy if needed
  int e1 = GameInfo::randomNumber(0,u1.energ);
  int e2 = GameInfo::randomNumber(0,u2.energ);
  int winner = u1.id_;
  if(e2 > e1) winner = u2.id_;

  //Kill unit, give points and subtract energy
  if(winner == u1.id_){
    killUnit(u2);
    info.points[u1.pl] += info.pointsPerUnit;
    u1.energ -= GameInfo::randomNumber(0,e2);
    if(u1.energ < info.energyMin) u1.energ = info.energyMin;
  }
  else{
    killUnit(u1);
    info.points[u2.pl] += info.pointsPerUnit;
    u2.energ -= GameInfo::randomNumber(0,e1);
    if(u2.energ < info.energyMin) u2.energ = info.energyMin;
  }
  return winner;
}

bool Board::executeOrder(int plId, Order ord){
  cerr << "executing order to " << ord.unitId << " owned by " << plId << " ";
  cerr << ord.dir << " " <<ord.type << " " << ord.unitId << endl;

  if(not unitOk(ord.unitId)){
    cerr << "error: unit " << ord.unitId << " is not valid" << endl;
    return false;
  }

  //Unit is valid
  Unit u = info.unitsVector[ord.unitId];
  if(u.pl != plId){
    if(not killedUnits[u.id_]) cerr << "error: unit " << u.id_ << " is not controlled by the player " << plId << endl;
    return false;
  }

  //Unit is controlled by the player
  if(ord.type == OrderType::movement){
    cerr << "movement" << endl;
    if(info.painter(u.p) != plId){ //Not on same-color domain
      if(ord.dir == Direction::DL or ord.dir == Direction::DR or ord.dir == Direction::UL or ord.dir == Direction::UR){
        cerr << "error: unit " << u.id_ << " cannot move diagonally here" << endl;
        return false;
      }
    }
    //Valid movement
    if(ord.dir == Direction::null) return true;
    Position newPos = u.p + ord.dir;
    Square sq = info.square_map[newPos.x][newPos.y];
    if(sq.hasUnit()){
      int win = fight(u,info.unitsVector[sq.u->id_]);
      if(win != u.id_){ //Lost the fight
        return false;
      }
    }
    else if(sq.hasBonus()){
      if(info.bonusPlayers[plId] < info.bonusMax and not info.unitsVector[u.id_].upg){
        info.bonusPlayers[plId]++;
        info.unitsVector[u.id_].upg = true;
      }
      info.square_map[newPos.x][newPos.y].b = nullptr;
    }
    info.unitsVector[u.id_].p = newPos;
    info.square_map[newPos.x][newPos.y].u = &info.unitsVector[u.id_];
    info.square_map[u.p.x][u.p.y].u = nullptr;
    draw(plId,u.id_,newPos,u.p);
    return true;
  }
  else if(ord.type == OrderType::attack){
    //Compute attacked position
    //If there is an enemy unit, attack
    //If there is not, see if there was one before that has moved
    //If there was, see if it is in range
    //If it still is in range, attack

  }
  else if(ord.type == OrderType::ability){
    if(not info.unitsVector[u.id_].upg){
      cerr << "unit " << u.id_ << " is not upgraded and cannot use the ability" << endl;
      return false;
    }
    
    //The ability can be used
  }
  cerr << "didn't do shit" << endl;
  return false;
}

void Board::executeRound(const vector<Player*>& pl){

  if(info.round() == 40){
    //fills some squares. used to test a specific case you cannot generate alone
    //4,5,6
    info.square_map[4][1].plPainter = 0;
    info.square_map[5][1].plPainter = 0;
    info.square_map[6][1].plPainter = 0;
    info.square_map[4][1].isBorder = true;
    info.square_map[5][1].isBorder = true;
    info.square_map[6][1].isBorder = true;

    info.square_map[5][3].plPainter = 2;
    info.square_map[5][3].isBorder = true;
  }

  killedUnits = vector<bool>(info.unitsMax*info.numPlayers,false);
  for(int i = 0; i < pl.size(); ++i){
    for(int j = 0; j <= pl[i]->index; ++j){
      if(executeOrder(i,pl[i]->orderList[j])){
        cerr << "order executed succesfully" << endl;
      }
      else{
        cerr << "order didn't execute" << endl;
      }
    }
  }
}

void Board::printRound(){
  //cout << endl << "Printing round " << info.round() << endl << endl;
  cout << info.round() << " ";
  for(int i = 0; i < info.boardHeight; ++i){
    for(int j = 0; j < info.boardWidth; ++j){
      
      Square sq = info.square(Position(i,j));
      //Prints painter id, drawer id and unit's player id
      cout << sq.plPainter << " " << sq.plDrawer << " ";
      if(sq.hasUnit()) cout << sq.unit().pl << " ";
      else cout << -1 << " ";
    }
  }
  cout << endl;

  /*if(info.round() == 36){
    Square sq = info.square(Position(3,5));
    Square sq2 = info.square(Position(3,4));
    if(sq.hasUnit()) cerr << "sq has unit" << endl;
    if(sq2.hasUnit()) cerr << "sq2 has unit" << endl;
  }*/

  for(int i = 0; i < info.boardHeight; ++i){
    for(int j = 0; j < info.boardWidth; ++j){
      
      Square sq = info.square(Position(i,j));
      if(sq.painted()) cerr << sq.plPainter;
      else if(sq.drawed()) cerr << 'd';
      else cerr << '.';
      if(j < info.boardWidth-1) cerr << " ";
    }
    cerr << endl;
  }
  cerr << endl;
}

void Board::printSettings(){info.printSettings();}