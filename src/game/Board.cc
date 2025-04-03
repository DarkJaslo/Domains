#include "Board.hh"
#include "Encoding.hh"
using namespace std;

/*
  Board class. Contains most of the game's logic. I programmed most of it in May-June 2023
*/


//GameInfo random stuff implementation

/* Random engine based in pi's decimals. This was an intrusive thought and works just fine for the game.
  I discourage players from taking advantage on this in any form: just make a good player instead.
  
  With that said, enjoy: */
const char pi[]= "3141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844609550582231725359408128481117450284102701938521105559644622948954930381964428810975665933446128475648233786783165271201909145648566923460348610454326648213393607260249141273724587006606315588174881520920962829254091715364367892590360011330530548820466521384146951941511609433057270365759591953092186117381932611793105118548074462379962749567351885752724891227938183011949129833673362440656643086021394946395224737190702179860943702770539217176293176752384674818467669405132000568127145263560827785771342757789609173637178721468440901224953430146549585371050792279689258923542019956112129021960864034418159813629774771309960518707211349999998372978049951059731732816096318595024459455346908302642522308253344685035261931188171010003137838752886587533208381420617177669147303598253490428755468731159562863882353787593751957781857780532171226806613001927876611195909216420198938095257201065485863278865936153381827968230301952035301852968995773622599413891249721775283479131515574857242454150695950829533116861727855889075098381754637464939319255060400927701671139009848824012858361603563707660104710181942955596198946767837449448255379774726847104047534646208046684259069491293313677028989152104752162056966024058038150193511253382430035587640247496473263914199272604269922796782354781636009341721641219924586315030286182974555706749838505494588586926995690927210797509302955321165344987202755960236480665499119881834797753566369807426542527862551818417574672890977772793800081647060016145249192173217214772350141441973568548161361157352552133475741849468438523323907394143334547762416862518983569485562099219222184272550254256887671790494601653466804988627232791786085784383827967976681454100953883786360950680064225125205117392984896084128488626945604241965285022210661186306744278622039194945047123713786960956364371917287467764657573962413890865832645995813390478027590099465764078951269468398352595709825822620522489407726719478268482601476990902640136394437455305068203496252451749399651431429809190659250937221696461515709858387410597885959772975498930161753928468138268683868942774155991855925245953959431049972524680845987273644695848653836736222626099124608051243884390451244136549762780797715691435997700129616089441694868555848406353422072225828488648158456028506016842739452267467678895252138522549954666727823986456596116354886230577456498035593634568174324112515076069479451096596094025228879710893145669136867228748940560101503308617928680920874760917824938589009714909675985261365549781893129784821682998948722658804857564014270477555132379641451523746234364542858444795265867821051141354735739523113427166102135969536231442952484937187110145765403590279934403742007310578539062198387447808478489683321445713868751943506430218453191048481005370614680674919278191197939952061419663428754440643745123718192179998391015919561814675142691239748940907186494231961567945208095146550225231603881930142093762137855956638937787083039069792077346722182562599661501421503068038447734549202605414665925201497442850732518666002132434088190710486331734649651453905796268561005508106658796998163574736384052571459102897064140110971206280439039759515677157700420337869936007230558763176359421873125147120532928191826186125867321579198414848829164470609575270695722091756711672291098169091528017350671274858322287183520935396572512108357915136988209144421006751033467110314126711136990865851639831501970165151168517143765761835155650884909989859982387345528331635507647918535893226185489632132933089857064204675259070915481416549859461637180270981994309924488957571282890592323326097299712084433573265489382391193259746366730583604142813883032038249037589852437441702913276561809377344403070746921120191302033038019762110110044929321516084244485963766983895228684783123552658213144957685726243344189303968642624341077322697802807318915441101044682325271620105265227211166039666557309254711055785376346682065310989652691862056476931257058635662018558100729360659876486117910453348850346113657686753249441668039626579787718556084552965412665408530614344431858676975145661406800700237877659134401712749470420562230538994561314071127000407854733269939081454664645880797270826683063432858785698305235808933065757406795457163775254202114955761581400250126228594130216471550979259230990796547376125517656751357517829666454779174501129961489030463994713296210734043751895735961458901938971311179042978285647503203198691514028708085990480109412147221317947647772622414254854540332157185306142288137585043063321751829798662237172159160771669254748738986654949450114654062843366393790039769265672146385306736096571209180763832716641627488880078692560290228472104031721186082041900042296617119637792133757511495950156604963186294726547364252308177036751590673502350728354056704038674351362222477158915049530984448933309634087807693259939780541934144737744184263129860809988868741326047215695162396586457302163159819319516735381297416772947867242292465436680098067692823828068996400482435403701416314965897940924323789690706977942236250822168895738379862300159377647165122893578601588161755782973523344604281512627203734314653197777416031990665541876397929334419521541341899485444734567383162499341913181480927777103863877343177207545654532207770921201905166096280490926360197598828161332316663652861932668633606273567630354477628035045077723554710585954870279081435624014517180624643626794561275318134078330336254232783944975382437205835311477119926063813346776879695970309833913077109870408591337464144282277263465947047458784778720192771528073176790770715721344473060570073349243693113835049316312840425121925651798069411352801314701304781643788518529092854520116583934196562134914341595625865865570552690496520985803385072242648293972858478316305777756068887644624824685792603953527734803048029005876075825104747091643961362676044925627420420832085661190625454337213153595845068772460290161876679524061634252257719542916299193064553779914037340432875262888963995879475729174642635745525407909145135711136941091193932519107602082520261879853188770584297259167781314969900901921169717372784768472686084900337702424291651300500516832336435038951702989392233451722013812806965011784408745196012122859937162313017114448464090389064495444006198690754851602632750529834918740786680881833851022833450850486082503930213321971551843063545500766828294930413776552793975175461395398468339363830474611996653858153842056853386218672523340283087112328278921250771262946322956398989893582116745627010218356462201349671518819097303811980049734072396103685406643193950979019069963955245300545058068550195673022921913933918568034490398205955100226353536192041994745538593810234395544959778377902374216172711172364343543947822181852862408514006660443325888569867054315470696574745855033232334210730154594051655379068662733379958511562578432298827372319898757141595781119635833005940873068121602876496286744604774649159950549737425626901049037781986835938146574126804925648798556145372347867330390468838343634655379498641927056387293174872332083760112302991136793862708943879936201629515413371424892830722012690147546684765357616477379467520049075715552781965362132392640616013635815590742202020318727760527721900556148425551879253034351398442532234157623361064250639049750086562710953591946589751413103482276930624743536325691607815478181152843667957061108615331504452127473924544945423682886061340841486377670096120715124914043027253860764823634143346235189757664521641376796903149501910857598442391986291642193994907236234646844117394032659184044378051333894525742399508296591228508555821572503107125701266830240292952522011872676756220415420516184163484756516999811614101002996078386909291603028840026910414079288621507842451670908700069928212066041837180653556725253256753286129104248776182582976515795984703562226293486003415872298053498965022629174878820273420922224533985626476691490556284250391275771028402799806636582548892648802545661017296702664076559042909945681506526530537182941270336931378517860904070866711496558343434769338578171138645587367812301458768712660348913909562009939361031029161615288138437909904231747336394804575931493140529763475748119356709110137751721008031559024853090669203767192203322909433467685142214477379393751703443661991040337511173547191855046449026365512816228824462575916333039107225383742182140883508657391771509682887478265699599574490661758344137522397096834080053559849175417381883999446974867626551658276584835884531427756879002909517028352971634456212964043523117600665101241200659755851276178583829204197484423608007193045761893234922927965019875187212726750798125547095890455635792122103334669749923563025494780249011419521238281530911407907386025152274299581807247162591668545133312394804947079119153267343028244186041426363954800044800267049624820179289647669758318327131425170296923488962766844032326092752496035799646925650493681836090032380929345958897069536534940603402166544375589004563288225054525564056448246515187547119621844396582533754388569094113031509526179378002974120766514793942590298969594699556576121865619673378623625612521632086286922210327488921865436480229678070576561514463204692790682120738837781423356282360896320806822246801224826117718589638140918390367367222088832151375560037279839400415297002878307667094447456013455641725437090697939612257142989467154357846878861444581231459357198492252847160504922124247014121478057345510500801908699603302763478708108175450119307141223390866393833952942578690507643100638351983438934159613185434754649556978103829309716465143840700707360411237359984345225161050702705623526601276484830840761183013052793205427462865403603674532865105706587488225698157936789766974220575059683440869735020141020672358502007245225632651341055924019027421624843914035998953539459094407046912091409387001264560016237428802109276457931065792295524988727584610126483699989225690";
const int permutations[24][4] = {{0,1,2,3},{0,1,3,2},{0,2,1,3},{0,2,3,1},{0,3,1,2},{0,3,2,1},{1,0,2,3},{1,0,3,2},{1,2,0,3},{1,2,3,0},{1,3,0,2},{1,3,2,0},{2,0,1,3},{2,0,3,1},{2,1,0,3},{2,1,3,0},{2,3,0,1},{2,3,1,0},{3,0,1,2},{3,0,2,1},{3,1,0,2},{3,1,2,0},{3,2,0,1},{3,2,1,0}};
int jump;
int randind;
int randsize(){return 9973;}
int randjump(){return 11;}
int randdigit()
{
  randind += jump;
  if(randind>=randsize()) randind%=randsize();
  char c = pi[randind];
  c-='0';
  return c;
}
int GameInfo::randomNumber(int l, int r)
{
  if(l == r) return l;
  //std::cerr << "starting random number generation" << std::endl;
  int div = 100000;
  int digits = 5;  
  int number = 0;
  for(int i = digits; i > 0; --i)
  {
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
vector<int> GameInfo::randomPermutation()
{
  vector<int> v(4);
  int index = randomNumber(0,23);
  v[0] = permutations[index][0];
  v[1] = permutations[index][1];
  v[2] = permutations[index][2];
  v[3] = permutations[index][3];
  return v;
}

//Utility functions
const vector<Direction> NORMAL_DIRS = {Direction::up, Direction::down, Direction::left, Direction::right};
bool isDiagonal(Direction d){return d >= Direction::UL;}
bool Board::unitOk(int uid)const {return uid >= 0 and uid < static_cast<int>(info.unitsVector.size());}
template <typename T>
bool findInVector(const T& thing, const vector<T>& vec)
{
  for(const T& t : vec) if(t == thing) return true;
  return false;
}
Direction inverse(Direction d)
{
  switch(d){
    case Direction::left:
      return Direction::right;
      break;
    case Direction::right:
      return Direction::left;
      break;
    case Direction::up:
      return Direction::down;
      break;
    case Direction::down:
      return Direction::up;
      break;
    case Direction::DL:
      return Direction::UR;
      break;
    case Direction::UR:
      return Direction::DL;
      break;
    case Direction::UL:
      return Direction::DR;
      break;
    case Direction::DR:
      return Direction::UL;
    default:
      return Direction::null;
      break;
  }
}
void printMatrix(Matrix<Square>& map)
{
  cerr << "   ";
  for(int i = 0; i < map.cols(); ++i)
  {
    cerr << i << " ";
  }
  cerr << "\n";
  for(int i = 0; i < map.rows(); ++i)
  {
    cerr << i;
    if(i < 10) cerr << " ";
    cerr << " ";
    for(int j = 0; j < map.cols(); ++j)
    {
      if(map[Position(i,j)].drawn())
      {
        cerr << 'd';
      }
      else if(map[Position(i,j)].painter() == -1) cerr << '.';
      else cerr << map[Position(i,j)].painter();
      cerr << " ";
      if(j >= 9) cerr << " ";
    }
    cerr << "\n";
  }
  cerr << "\n";
}

//Board class
Board::~Board()
{
  //std::cerr << "destructing Board" << std::endl;
}
Board::Board(){debug = false; view = true; info.numPlayers = 4; }
Board::Board(bool d, bool v, int nplayers){debug = d; view = v; info.numPlayers = nplayers;}

void Board::iniBoard(int s, int rounds)
{
  if(debugBasic) std::cerr << "Initializing board...\n";
  seed = s;
  randind = seed%randsize();
  jump = seed%randjump()+1;   //+1 to avoid jump = 0
  if(debugBasic) std::cerr << "Reading settings...\n";
  info.maxRounds = rounds;
  info.readSettings();

  if(debugBasic) std::cerr << "Initializing squares...\n";

  info.square_map = Matrix<Square>(info.boardHeight,info.boardWidth);

  //Initialize squares
  for(int i = 0; i < info.square_map.rows(); ++i){
    for(int j = 0; j < info.square_map.cols(); ++j)
    {
      Square sq;
      Position ij(i,j);
      sq.p = ij;
      sq.uDrawer = -1;
      sq.plPainter = -1;
      sq.isBorder = false;
      sq.isAbility = false;
      sq.plDrawer = -1;
      sq.u = nullptr;
      sq.b = nullptr;
      sq.bb = nullptr;
      sq.closes = false;
      info.square_map[ij] = sq;
    }
  }
  info.old_square_map = info.square_map;

  if(debugBasic) std::cerr << "Initializing units...\n";

  info.unitsVector.reserve(info.unitsMax*info.numPlayers);
  drawStarts = std::vector<std::pair<Position,Position>>(info.unitsMax*info.numPlayers,make_pair(Position(-1,-1),Position(-1,-1)));

  //Initializes units
  for(int i = 0; i < info.unitsMax*info.numPlayers; ++i){
    info.unitsVector.emplace_back(
    Unit(/*id*/          i,
         /*player*/     -1,
         /*position*/   Position(-1,-1), //invalid position
         /*upgraded*/   false,
         /*energy*/     info.energyStart
    ));
  }
  if(debugBasic) std::cerr << "Spawning players...\n";
  spawnPlayers();
  if(debugBasic) std::cerr << "Board successfully initialized\n";
}

void Board::i_erasePath(int uid, Position p)
{
  if(info.posOk(p) and info.square_map[p].uDrawer == uid)
  {
    if(debugDrawErase) std::cerr << "erased drawing in " << p << "\n";
    Square sq = info.square(p);
    sq.uDrawer = -1;
    sq.plDrawer = -1;
    info.square_map[p] = sq;

    i_erasePath(uid,p+Direction::up);
    i_erasePath(uid,p+Direction::down);
    i_erasePath(uid,p+Direction::left);
    i_erasePath(uid,p+Direction::right);
  }
}

void Board::erasePath(int uid, Position p)
{
  if(debugDrawErase) std::cerr << "erasing in " << p << "(uid: " << uid << ")\n";
  if(debugDrawErase) std::cerr << "erasing starting position by unit " << uid << "\n";
  drawStarts[uid].first = Position(-1,-1);

  i_erasePath(uid,p);
}

void Board::killUnit(Unit& u)
{
  if(debug) std::cerr << "killing unit " << u.id() << " by " << u.player() << "\n";

  if(u.upgraded()) //Killed before using the ability
  {
    info.bonusPlayers[u.player()]--;
    u.upg = false;
  }
  u.pl = -1;
  info.square_map[u.p].u = nullptr;
  killedUnits[u.id()] = true;

  //Erase drawing if there is
  if(drawStarts[u.id()].first == Position(-1,-1)) return;
  erasePath(u.id(),drawStarts[u.id()].second);
}

void Board::deenclose(Position p){
  if(not info.posOk(p)) return;
  if(not info.square(p).closes) return;
  
  info.square_map[p].closes = false;
  deenclose(p+Direction::up);
  deenclose(p+Direction::down);
  deenclose(p+Direction::left);
  deenclose(p+Direction::right);
}

void Board::enclose(int plId, int uid, Position p, int& xmin, int& xmax, int& ymin, int& ymax)
{
  if(not info.posOk(p)) return;
  if(info.square(p).closes) return;
  if(info.square(p).uDrawer == uid or (info.square(p).painter() == plId and info.square(p).border()))
  {
    info.square_map[p].closes = true;
    if(not info.square(p).border()){
      info.square_map[p].isBorder = true;
    }
    if(p.x < xmin) xmin = p.x;
    if(p.x > xmax) xmax = p.x;
    if(p.y < ymin) ymin = p.y;
    if(p.y > ymax) ymax = p.y;

    enclose(plId,uid,p+Direction::up,   xmin,xmax,ymin,ymax);
    enclose(plId,uid,p+Direction::down, xmin,xmax,ymin,ymax);
    enclose(plId,uid,p+Direction::left, xmin,xmax,ymin,ymax);
    enclose(plId,uid,p+Direction::right,xmin,xmax,ymin,ymax);
  }
}

//used to flood
#define COLORINDEX 10

void Board::flood(int plId, int uid, int col, Position p, bool& correct, Matrix<Square>& grid){
  //borders are drawings and painted squares
  if(p.x < 0 or p.y < 0 or p.x >= grid.rows() or p.y >= grid.cols())
  {
    if(correct) correct = false;
    return;
  }
  //Else,
  Square sq = grid[p];
  if((sq.painter() >= COLORINDEX) or (sq.uDrawer == uid) or (sq.painter() == plId))
  {
    if(debugFlood){
      if(sq.painter() == plId) std::cerr << "found border at " << p << "\n";
      else if(sq.plDrawer == plId) std::cerr << "found drawing at " << p << "\n";
    }
    return;
  }
  //Else,
  if(debugFlood) std::cerr << "flooded position " << int(p.x) << "," << int(p.y) << " with col " << col << "\n";
  
  grid[p].plPainter = col;
  flood(plId,uid,col,p+NORMAL_DIRS[0],correct,grid);
  flood(plId,uid,col,p+NORMAL_DIRS[1],correct,grid);
  flood(plId,uid,col,p+NORMAL_DIRS[2],correct,grid);
  flood(plId,uid,col,p+NORMAL_DIRS[3],correct,grid);
}

void Board::perpendicularDirections(Direction dir, Direction& res1, Direction& res2)
{
  if(dir == Direction::left or dir == Direction::right)
  {
    res1 = Direction::up;
    res2 = Direction::down;
  }
  else if(dir == Direction::up or dir == Direction::down)
  {
    res1 = Direction::left;
    res2 = Direction::right;
  }
}

Position Board::followDrawing(int uid, Position act, Position ant, Matrix<Square>& grid)
{
  if(act == Position(-1,-1)) return act;

  Direction d = act.to(ant);
  Position next;

  for(int i = 0; i < 4; ++i)
  {
    if(NORMAL_DIRS[i] == d) continue;

    Position aux = act+NORMAL_DIRS[i];
    if(aux.x < 0 or aux.y < 0 or aux.x >= grid.rows() or aux.y >= grid.cols()) continue;

    Square sq = grid[aux];
    if(sq.uDrawer == uid){ return aux; }
  }

  //If it has not returned, it means end of drawing
  return Position(-1,-1);
}

void Board::paint(int plId, int uid, Position in, Position out){

  /*Follows the drawing and tries to flood in all directions that are not drawn*/

  if(debugPaint) cerr << "Painting at " << in << "\n";

  //Calculates the maximum subsection of the grid to paint
  int xmin,xmax,ymin,ymax;
  xmin = xmax = out.x;
  ymin = ymax = out.y;
  enclose(plId,uid,out,xmin,xmax,ymin,ymax);
  deenclose(out);

  if(debugPaint) cerr << "Enclosed perimeter: " << xmin << "," << ymin << " " << xmax << "," << ymax << "\n";

  int rows = xmax-xmin+1;
  int cols = ymax-ymin+1;
  Matrix<Square> map(rows,cols);

  //Copies such subsection
  for(int i = xmin; i <= xmax; ++i)
  {
    for(int j = ymin; j <= ymax; ++j)
    {
      Position mapPos(i-xmin,j-ymin);
      Position squareMapPos(i,j);
      map[mapPos] = info.square_map[squareMapPos];
    }
  }

  if(debugPaint) printMatrix(map);

  //Erase starting position oh this painter unit's drawing
  drawStarts[uid].first = Position(-1,-1);

  //Variables to follow the drawing and flood
  Position act = Position(out.x-xmin,out.y-ymin);
  Position ant = Position(in.x-xmin,in.y-ymin);
  Position next = followDrawing(uid,act,ant,map);

  if(debugPaint)
  {
    std::cerr << "Followed drawing #1\n";
    std::cerr << "next: "<< next << "\n";
  }

  //Visited list to avoid loops
  vector<vector<bool>> visitedNext(xmax-xmin+1,vector<bool>(ymax-ymin+1,false));

  vector<int> validColors; //contains valid flood colors
  int colAct = COLORINDEX;

  //Follows the drawings and tries to flood

  while(act != Position(-1,-1) and not visitedNext[act.x][act.y])
  {  
    visitedNext[act.x][act.y] = true;
    Direction d1 = Direction::null;
    Direction d2 = Direction::null;
    perpendicularDirections(act.to(ant),d1,d2);

    if(d1 == Direction::null or d2 == Direction::null)
    {
      std::cerr << "wrong perpendicularDirections() output, wtf\n";
      exit(1);
    }

    Direction d3 = inverse(act.to(ant));
    vector<Direction> floodDirs;
    floodDirs.reserve(3);
    floodDirs.emplace_back(d1);
    floodDirs.emplace_back(d2);
    floodDirs.emplace_back(d3);

    //Tries to flood
    for(Direction d : floodDirs)
    {
      if(d == act.to(ant)) continue;

      Position floodPos = act+d;
      if(floodPos.x < 0 or floodPos.y < 0 or floodPos.x >= map.rows() or floodPos.y >= map.cols()) continue;
      if(map[floodPos].uDrawer == uid) continue;

      bool valid = true;
      flood(plId,uid,colAct,floodPos,valid,map);
      if(valid) { validColors.push_back(colAct); }
      ++colAct;
    }

    ant = act;
    act = next;
    next = followDrawing(uid,act,ant,map);
  }

  //Paints correctly flooded squares
  for(int i = 0; i < map.rows(); ++i){
    for(int j = 0; j < map.cols(); ++j)
    {
      Position pos(i,j);
      Position sqpos(i+xmin,j+ymin);
      Square sq = map[pos];

      if(sq.uDrawer == uid)
      {
        map[pos].plPainter = plId;
        map[pos].uDrawer = -1;
        map[pos].plDrawer = -1;
        continue;
      }

      if(sq.painter() < COLORINDEX) continue;
      
      if(findInVector(sq.painter(),validColors))
      {
        map[pos].plPainter = plId;

        //Erases start of possible drawings
        for(int i = 0; i < drawStarts.size(); ++i)
        {
          if(drawStarts[i].first == Position(pos.x+xmin,pos.y+ymin))
          {
            if(debugDrawErase) cerr << "erasing drawing starting in " << Position(pos.x+xmin,pos.y+ymin) << "\n";
            erasePath(i,drawStarts[i].second);
          }
        }

        //Erases all drawings
        if(info.square(sqpos).drawn() and info.square(sqpos).uDrawer != uid)
        {
          if(debug) std::cerr << std::endl << "erasing path that started at " << i <<"," << j << "\n\n";
          erasePath(info.square(sqpos).uDrawer,sqpos);
        }
      }
    }
  }

  //Copies the painted squares back
  for(int i = xmin; i <= xmax; ++i){
    for(int j = ymin; j <= ymax; ++j)
    {
      Position mapPos(i-xmin,j-ymin);
      Position squareMapPos(i,j);
      Square sq = info.square_map[squareMapPos];

      if(map[mapPos].painter() == plId and sq.painter() != plId)
      {
        sq.plPainter = plId;
      }
      if(sq.uDrawer == uid)
      {
        sq.plPainter = plId;
        sq.uDrawer = -1;
        sq.plDrawer = -1;
      }
      info.square_map[squareMapPos] = sq;      
    }
  }

  //Updates borders
  if(debugPaint) std::cerr << "updating borders...\n";
  for(int i = xmin-1; i <= xmax+1; ++i){
    for(int j = ymin-1; j <= ymax+1; ++j)
    {
      Position pos = Position(i,j);
      if(not info.posOk(pos)) continue;
      if(info.square_map[pos].plPainter != plId) continue;
      
      bool border = false;
      for(int index = 0; index < 1; ++index)
      {
        Position pos2 = pos+Direction::UL;
        if(info.posOk(pos2) and info.square_map[pos2].plPainter != plId){border = true;break;}
        pos2 = pos+Direction::up;
        if(info.posOk(pos2) and info.square_map[pos2].plPainter != plId){border = true;break;}
        pos2 = pos+Direction::UR;
        if(info.posOk(pos2) and info.square_map[pos2].plPainter != plId){border = true;break;}
        pos2 = pos+Direction::left;
        if(info.posOk(pos2) and info.square_map[pos2].plPainter != plId){border = true;break;}
        pos2 = pos+Direction::right;
        if(info.posOk(pos2) and info.square_map[pos2].plPainter != plId){border = true;break;}
        pos2 = pos+Direction::DL;
        if(info.posOk(pos2) and info.square_map[pos2].plPainter != plId){border = true;break;}
        pos2 = pos+Direction::down;
        if(info.posOk(pos2) and info.square_map[pos2].plPainter != plId){border = true;break;}
        pos2 = pos+Direction::DR;
        if(info.posOk(pos2) and info.square_map[pos2].plPainter != plId){border = true;break;}
      }
      
      if(border){
        if(not info.square_map[pos].border()) info.square_map[pos].isBorder = true;
      }
      else{
        if(info.square_map[pos].border()) info.square_map[pos].isBorder = false;
      }
    }
  }
  if(debug) std::cerr << "paint function end\n";
}

void Board::draw(int plId, int uid, Position pnew, Position pant, Direction dir)
{
  if(debug) std::cerr << "unit " << uid << " of player " << plId << " drawing at " << pnew << "\n";

  Square sant = info.square(pant);
  Square snew = info.square(pnew);

  if(debug) std::cerr << "sant: " << sant.pos() << ", snew: " << snew.pos() << "\n";
  
  if(snew.drawn() /*and snew.uDrawer != uid*/)
  {
    if(debug) std::cerr << "new pos is drawn\n";
    //if you step on someone else's drawing, erase it
    if(debug) std::cerr << "erasing path at " << snew.pos() << "\n";
    erasePath(snew.uDrawer,pnew);
    sant = info.square(pant);
    snew = info.square(pnew);
  }
  if(sant.uDrawer == uid)
  {
    if(debug) std::cerr << "there is a drawing behind\n";
    //draws if behind you is a drawing
    info.square_map[pnew].uDrawer = uid;
    info.square_map[pnew].plDrawer = plId;
    //if behind you is a drawing and you enter a secure painted area, enclose and paint
    if(snew.plPainter == plId)
    {
      paint(plId,uid,snew.p,sant.p);
      return;
    }
  }
  if(sant.plPainter == plId and snew.plPainter != plId)
  {
    if(debug) std::cerr << "exiting painted area\n";
    //draws if exiting a secure painted area
    if(not isDiagonal(dir))
    {
      info.square_map[pnew].uDrawer = uid;
      info.square_map[pnew].plDrawer = plId;

      //Adds to the map
      if(drawStarts[uid].first != Position(-1,-1))
      {
        if(debug)
        {
          std::cerr << "unit: " << uid << " ";
          std::cerr << "position " << pant <<  " already starts a drawing\n";
          printStartSquares(drawStarts);
        }
      }
      drawStarts[uid] = make_pair(pant,pnew);
    }
  }
}

//Assumes u1 is always the attacker
int Board::fight(Unit& u1, Unit& u2, FightMode fm)
{
  if(debug) std::cerr << "fighting, ";

  int e1 = 0;
  int e2 = 0;
  int winner = -1;

  if(fm == FightMode::Fair)  //Uses random numbers to decide
  {  
    e1 = GameInfo::randomNumber(0,u1.energ);
    e2 = GameInfo::randomNumber(0,u2.energ);
    if(e1 > e2) winner = u1.id();
    else if(e2 > e1) winner = u2.id();
    else //Equal energies
    {
      if(GameInfo::randomNumber(0,1)) winner = u1.id();
      else winner = u2.id();
    }
  }
  else if(fm == FightMode::Attacks)  //Kills. Assumes u1 is always the attacker
  {  
    winner = u1.id();
  }

  if(debug) std::cerr << "winner: " << winner << "\n";

  //Kill unit, give points and subtract energy
  if(winner == u1.id())
  {
    killUnit(u2);
    info.playerPoints[u1.player()] += info.pointsPerUnit;
    if(fm == FightMode::Fair)
    {
      u1.energ -= GameInfo::randomNumber(0,e2/2);
      if(u1.energ < info.energyMin) u1.energ = info.energyMin;
    }
  }
  else if(winner == u2.id())
  {
    killUnit(u1);
    info.playerPoints[u2.player()] += info.pointsPerUnit;
    if(fm == FightMode::Fair)
    {
      u2.energ -= GameInfo::randomNumber(0,e1/2);
      if(u2.energ < info.energyMin) u2.energ = info.energyMin;
    }
  }
  return winner;
}

bool Board::executeOrder(int plId, Order ord)
{
  if(debugOrders) std::cerr << "executing order to " << ord.unitId << " owned by " << plId << " ";
  if(debugOrders) std::cerr << ord.dir << " " << ord.type << " " << ord.unitId << "\n";

  if(not unitOk(ord.unitId))
  {
    std::cerr << "warning: unit " << ord.unitId << " is not valid\n";
    return false;
  }

  //Unit is valid
  Unit u = info.unitsVector[ord.unitId];
  if(u.pl != plId)
  {
    if(not killedUnits[u.id()]) std::cerr << "warning: unit " << u.id() << " is not controlled by the player " << plId << "\n";
    return false;
  }

  //Unit is controlled by the player
  switch (ord.type){
  case OrderType::movement:{
    
    if(info.square(u.p).painter() != plId)
    { //Not on same-color domain
      if(isDiagonal(ord.dir))
      {
        std::cerr << "warning: unit " << u.id() << " by player " << u.player() << " cannot move diagonally here (pos: " << u.position() << ")\n";
        return false;
      }
    }

    //Valid movement
    if(ord.dir == Direction::null) return true;
    Position newPos = u.position() + ord.dir;
    Position actPos = u.position();

    //Return false if position is not valid
    if(not info.posOk(newPos))
    {
      std::cerr << "warning: player " << u.player() << "'s unit " << u.id()  << " tried to move to outside the board\n";
      return false;
    }
    
    Square sq = info.square(newPos);
    Square act = info.square(u.position());

    //Prevents exiting ability areas
    if(act.ability() and not sq.ability() and act.painted() and act.painter() != u.player())
    {
      return false;
    }

    //Prevents entering ability areas
    if(((not act.ability()) or (act.ability() and act.painter() == u.player())) and sq.ability() and sq.painted() and sq.painter() != u.player())
    {
      return false;
    }

    if(sq.hasUnit()){ //Fight

      if(sq.unit().player() == u.player()) return false; 

      FightMode fm = FightMode::Fair;
      if(isDiagonal(sq.p.to(u.p)) and sq.unit().player() != sq.painter())
        fm = FightMode::Attacks;

      int win = fight(info.unitsVector[u.id()],info.unitsVector[sq.u->id()],fm);
      if(win != u.id()){ //Lost the fight
        return true;
      }
    }
    else if(sq.hasBonus()) //Try to take -> consume
    { 
      if(info.bonusPlayers[plId] < info.bonusPerPlayer and not info.unitsVector[u.id()].upgraded())
      {
        info.bonusPlayers[plId]++;
        info.unitsVector[u.id()].upg = true;
      }
      info.square_map[newPos].b = nullptr;
      info.currentBonuses--;
      int bid = sq.bonus().id;
      info.bonusVector[bid].p = Position(-1,-1);
      info.bonusCounters[bid] = GameInfo::randomNumber(0,info.roundsPerBonus)-1;
    }
    else if(sq.hasBubble()) //Pop if ally, change colour and activate rounds to pop if enemy
    { 
      Bubble bb = sq.bubble();

      if(bb.player() == u.player())
      {
        popBubble(bb.position(),false);
      }
      else{
        bb.pl = u.player();
        bb.rtp = info.roundsToPop;
        info.bubblesVector[bb.id_] = bb;
        return false;
      }
    }

    //Move
    info.unitsVector[u.id()].p = newPos;
    info.square_map[newPos].u = &info.unitsVector[u.id()];
    info.square_map[actPos].u = nullptr;
    draw(plId,u.id_,newPos,u.p,ord.dir);
    return true;
    break;
  }
  case OrderType::attack:{

    //Compute attacked position
    Position attacked = u.p + ord.dir;
    if(not info.posOk(attacked))
    {
      std::cerr << "warning: unit " << u.id() << "by player " << u.player() <<  " attacked the invalid position" << attacked << "\n";
      return false;
    }
    Square sq = info.square(attacked);

    //If there is an enemy unit, attack
    if(sq.hasUnit())
    {
      if(sq.unit().player() == u.player()){ //Can't slaughter one's allies
        return false;
      }

      //Decide fairness
      FightMode fm = FightMode::Fair;
      if(isDiagonal(ord.dir))
      {
        if(sq.p.x != u.p.x and sq.p.y != u.p.y and sq.painter() != sq.unit().player())
        {
          fm = FightMode::Attacks;
        }
      }

      fight(u,info.unitsVector[sq.u->id()],fm);
      return true;
    }
    else if(sq.hasBubble())
    {
      if(debugOrders) std::cerr << "square has bubble\n";
      Bubble bb = sq.bubble();

      if(bb.player() == u.player())
      {
        popBubble(bb.position(),false);
      }
      else
      {
        bb.pl = u.player();
        bb.rtp = info.roundsToPop;
        info.bubblesVector[bb.id_] = bb;
      }
      return true;
    }
    
    //If there is nothing, see if there was someone before that has moved
    Square sqold = info.old_square_map[attacked];
    if(sqold.hasUnit())
    {
      //If there was, see if it is in range
      int uid = sqold.unit().id();

      Direction dirAux = Direction::null;

      vector<Direction> ALL_DIRS = NORMAL_DIRS;
      if(info.square(u.position()).painter() == u.player())
      {
        ALL_DIRS.push_back(Direction::UL);
        ALL_DIRS.push_back(Direction::UR);
        ALL_DIRS.push_back(Direction::DL);
        ALL_DIRS.push_back(Direction::DR);
      }
      for(Direction d : ALL_DIRS)
      {
        Position paux(u.position()+d);
        if(info.posOk(paux) and info.square(paux).hasUnit())
        {
          if(info.square(paux).unit().id() == uid)
          {
            dirAux = d;
            break;
          }
        }
      }

      if(dirAux != Direction::null) //If it still is in range, attack
      {
        //Decide fairness
        FightMode fm = FightMode::Fair;
        if(isDiagonal(ord.dir))
        {
          Square sqaux = info.square(sq.p+dirAux);
          if(isDiagonal(sqaux.p.to(u.p)) and sqaux.painter() != sqaux.unit().player())
          {
            fm = FightMode::Attacks;
          }
        }
        fight(u,info.unitsVector[sq.u->id()],fm);
        return true;
      }
      else //Free attack: at the end of all turns, looks again to see if someone has moved here and attacks
      {
        freeAttacks.push(FreeAttack(u.id(),u.position(),attacked));
        return true;
      }
    }
    break;
  }
  case OrderType::ability:{

    if(not info.unitsVector[u.id()].upgraded())
    {
      std::cerr << "player " << u.player() << ": unit " << u.id() << " is not upgraded and cannot use the ability\n";
      return false;
    }
    
    //The ability can be used

    //Consume ability
    info.unitsVector[u.id()].upg = false;
    info.bonusPlayers[u.player()]--;
    //Queue for later
    abilityUnits.push_back(u.id());
    break;
  }
  default:{
    std::cerr << "ordertype was wrong" << std::endl;
    break;
  }
  }
  return false;
}

void Board::performFreeAttacks()
{
  while(not freeAttacks.empty())
  {
    FreeAttack f = freeAttacks.front();
    freeAttacks.pop();

    if(killedUnits[f.uid]) continue;
    if(not info.square(f.tPos).hasUnit()) continue;
    if(info.square(f.tPos).unit().player() == info.square(f.uPos).unit().player()) continue;

    FightMode fm = FightMode::Fair;
    Square sqTarget = info.square(f.tPos);
    Square sqAttacker = info.square(f.uPos);

    if(isDiagonal(f.uPos.to(f.tPos)) and sqTarget.painter() != sqTarget.unit().player())
    {
      fm = FightMode::Attacks;
    }
    fight(*(sqAttacker.u),*(sqTarget.u),fm);
  }
}

void Board::popBubble(Position p, bool isForced)
{
  if(not info.posOk(p)) return;
  Square sq = info.square(p);
  if(not sq.hasBubble()) return;
  Bubble b = info.bubble(p);

  queue<Position> affected;
  vector<Position> toPaint = {p, p+Direction::up, p+Direction::down, p+Direction::left, p+Direction::right};

  for(int i = 0; i < toPaint.size(); ++i)
  {
    Position aux = toPaint[i];
    if(not info.posOk(aux)) continue;
    Square sqaux = info.square(aux);
    if(sqaux.ability()) continue;

    affected.push(aux);
    
    if(sqaux.painter() != b.player())
    {
      info.square_map[aux].plPainter = b.player();
    }
  }

  for(int i = 0; i < toPaint.size(); ++i)
  {
    Position aux = toPaint[i];
    if(not info.posOk(aux)) continue;
    Square sqaux = info.square(aux);
    if(not sqaux.drawn()) continue;
    
    if(sqaux.plDrawer == b.player())
    {
      if(debug) std::cerr << "painting from bubble pop at " << int(aux.x) << "," << int(aux.y) << "\n";

      //Find out drawing
      Position drawingOut(-1,-1);
      for(int i = 0; i < NORMAL_DIRS.size(); ++i)
      {
        Position aux2 = aux+NORMAL_DIRS[i];
        if(not info.posOk(aux2)) continue;
        Square sqaux2 = info.square_map[aux2];
        if(sqaux2.uDrawer == sqaux.uDrawer)
        {
          drawingOut = aux2;
          break;
        }
      }
      if(drawingOut == Position(-1,-1))
      {
        if(debug) std::cerr << "popBubble: didn't find a position" << std::endl;
        if(sqaux.drawn())
        {
          erasePath(sqaux.uDrawer,drawStarts[sqaux.uDrawer].second);
        }
        continue;
      }

      paint(b.player(),sqaux.uDrawer,aux,drawingOut);

      //Erase drawing
      sqaux = info.square(aux);
      if(sqaux.drawn())
      {
        erasePath(sqaux.uDrawer,drawStarts[sqaux.uDrawer].second);
      }
    }
    else if(sqaux.plDrawer != -1)
    {
      if(debug)
      {
        std::cerr << "at pos " << int(sqaux.pos().x) << "," << int(sqaux.pos().y) << ": "; 
        std::cerr << "call to erasePath from popBubble because plDrawer is " << int(sqaux.plDrawer);
        std::cerr << " and bubble is " << b.player() << "\n";
      }
      erasePath(sqaux.uDrawer,aux);
    }
  }

  vector<Position> affPos = {p+Direction::UL, p+Direction::UR, p+Direction::DL, p+Direction::DR, Position(p.x-2,p.y), Position(p.x+2,p.y), Position(p.x,p.y-2), Position(p.x,p.y+2)};

  for(Position p : affPos)
    if(info.posOk(p)) affected.push(p);

  vector<Direction> dirs = {Direction::up, Direction::down, Direction::left, Direction::right};

  while(not affected.empty())
  {
    Square sq = info.square(affected.front());
    affected.pop();

    for(int i = 0; i < dirs.size(); ++i)
    {
      Position aux = sq.pos()+dirs[i];
      if(not info.posOk(aux)) continue;
      Square sqaux = info.square(aux);

      if(sq.painter() != sqaux.painter())
      {
        info.square_map[sq.pos()].isBorder = true;
        info.square_map[aux].isBorder = true;
        break;
      }
    }
  }

  if(isForced){ //Done by someone else
    info.playerPoints[b.player()] += info.pointsPerPop;
  }
  else{
    info.playerPoints[b.player()] += info.pointsPerBubble;
  }

  info.square_map[b.position()].bb = nullptr;
  b.pl = -1;
  b.p = Position(-1,-1);
  b.rtp = 0;
  info.bubbleMovementCounters[b.id_] = 0;
  info.bubblesVector[b.id_] = b;
}

void Board::popBubbles()
{
  for(int i = 0; i < info.bubblesVector.size(); ++i)
  {
    if(info.bubblesVector[i].player() == -1) continue;
    if(info.bubblesVector[i].rtp == 0) continue;
    else if(--info.bubblesVector[i].rtp == 0)
    { //Pop bubble
      Position p = info.bubblesVector[i].position();
      popBubble(p,true);
    }
  }
}

void Board::invalidateAbility(int plId, Position p)
{
  if(info.posOk(p) and info.square(p).ability() and info.square(p).painter() == plId)
  {
    Square sq = info.square(p);
    sq.isAbility = false;
    sq.counter = 0;
    info.square_map[p] = sq;

    invalidateAbility(plId,p+Direction::up);
    invalidateAbility(plId,p+Direction::down);
    invalidateAbility(plId,p+Direction::left);
    invalidateAbility(plId,p+Direction::right);
  }
}

void Board::useAbility(int plId, Position p)
{
  int xmin = p.x-(info.abilitySize-1)/2;
  int xmax = p.x+(info.abilitySize-1)/2;
  int ymin = p.y-(info.abilitySize-1)/2;
  int ymax = p.y+(info.abilitySize-1)/2;

  if(debug)
  {
    std::cerr << "ability data: \n";
    std::cerr << "position: " << int(p.x) << "," << int(p.y) << "\n";
    std::cerr << "xmin: " << xmin << " " << "xmax: " << xmax << " " << "ymin: " << ymin << " " << "ymax: " << ymax << "\n";
  } 

  //Paint and highlight all squares as 'ability squares'
  //Erase all drawings made by enemies
  //Erase all ally-drawn squares if they are not border
  for(int i = xmin; i <= xmax; ++i){
    for(int j = ymin; j <= ymax; ++j)
    {
      Position pos(i,j);
      if(not info.posOk(pos)) continue;

      Square sq = info.square(pos);
      if(sq.ability() and sq.painter() != plId)
      {
        invalidateAbility(sq.painter(),pos);
      }
      if(sq.drawn())
      {
        if(sq.plDrawer != plId)
        {
          if(debug)
          {
            std::cerr << "square drawer is " << int(sq.plDrawer) << " but plId is " << plId << "\n";
            std::cerr << "erasing path\n";
          }          
          erasePath(sq.uDrawer,pos);
          sq = info.square(pos); //updating            
        }
        else if(sq.plDrawer == plId)
        {
          if(i != xmin and i != xmax and j != ymin and j != ymax)
          {
            if(drawStarts[sq.uDrawer].first != Position(-1,-1)) 
              drawStarts[sq.uDrawer].first = Position(-1,-1);
            sq.plDrawer = -1;
            sq.uDrawer = -1;
          }
        }
      }
      sq.isBorder = false;
      sq.plPainter = plId;
      sq.isAbility = true;
      sq.counter = info.abilityDuration+1; //+1 because unfortunately, counter is decremented after using abilities
      info.square_map[pos] = sq;
    }
  }

  //Prints ability
  if(debug)
  {
    std::cerr << "printing ability " << std::endl;
    for(int i = xmin; i <= xmax; ++i){
      for(int j = ymin; j <= ymax; ++j)
      {
        Position auxaux = Position(i,j);
        if(info.posOk(auxaux))
        {
          std::cerr << info.square_map[auxaux].painter() << " ";
        }
        else std::cerr << -9 << " ";
      }
      std::cerr << std::endl;
    }
  }

  //Looks at the border and makes it border if needed
  for(int i = ymin; i <= ymax; ++i)
  {
    Position p1(xmin,i);
    if(info.posOk(p1))
    {
      Square s1 = info.square(p1);
      Position up = p1+Direction::up;
      if(info.posOk(up))
      {
        Square sq = info.square(up);
        if(s1.painter() != sq.painter())
        {
          if(s1.painter() != -1)
          {
            s1.isBorder = true;
          }
          if(sq.painter() != -1)
          {
            sq.isBorder = true;
            info.square_map[up] = sq;
          }
        }
      }

      if(i == xmin or i == xmax) //Special case for corners
      {
        if(not s1.border())
        {
          Position spec = i == xmin ? p1+Direction::left : p1+Direction::right;
          if(info.posOk(spec)){
            Square sq = info.square(spec);
            if(s1.painter() != sq.painter())
            {
              if(s1.painter() != -1)
              {
                s1.isBorder = true;
              }
              if(sq.painter() != -1)
              {
                sq.isBorder = true;
              }
            }
            info.square_map[spec] = sq;
          }
        }
      }
      info.square_map[p1] = s1;
    }

    p1 = Position(xmax,i);
    if(info.posOk(p1))
    {
      Square s1 = info.square(p1);
      Position down = p1+Direction::down;
      if(info.posOk(down))
      {
        Square sq = info.square(down);
        if(s1.painter() != sq.painter())
        {
          if(s1.painter() != -1)
          {
            s1.isBorder = true;
          }
          if(sq.painter() != -1)
          {
            sq.isBorder = true;
          }
        }
        info.square_map[down] = sq;
      }

      if(i == xmin or i == xmax)
      { //Special case for corners
        if(not s1.border())
        {
          Position spec = i == xmin ? p1+Direction::left : p1+Direction::right;
          if(info.posOk(spec))
          {
            Square sq = info.square(spec);
            if(s1.painter() != sq.painter())
            {
              if(s1.painter() != -1)
              {
                s1.isBorder = true;
              }
              if(sq.painter() != -1)
              {
                sq.isBorder = true;
              }
            }
            info.square_map[spec] = sq;
          }
        }
      }
      info.square_map[p1] = s1;
    }  
  }
  for(int i = xmin+1; i <= xmax-1; ++i)
  {
    Position p1(i,ymin);
    if(info.posOk(p1))
    {
      Square s1 = info.square(p1);
      Position left = p1+Direction::left;
      if(info.posOk(left))
      {
        Square sq = info.square(left);
        if(s1.painter() != sq.painter())
        {
          if(s1.painter() != -1)
          {
            info.square_map[p1].isBorder = true;
          }
          if(sq.painter() != -1)
          {
            info.square_map[left].isBorder = true;
          }
        }
      }
      
    }
    p1 = Position(i,ymax);
    if(info.posOk(p1))
    {
      Square s1 = info.square(p1);
      Position right = p1+Direction::right;
      if(info.posOk(right))
      {
        Square sq = info.square(right);
        if(s1.painter() != sq.painter())
        {
          if(s1.painter() != -1)
          {
            info.square_map[p1].isBorder = true;
          }
          if(sq.painter() != -1)
          {
            info.square_map[right].isBorder = true;
          }
        }
      }
    }
  }

  //Paints if needed
  for(int i = xmin; i <= xmax; ++i){
    for(int j = ymin; j <= ymax; ++j)
    {
      Position pos(i,j);
      if(info.posOk(pos))
      {
        Square sq = info.square(pos);
        if((pos.x == xmin or pos.x == xmax or pos.y == ymin or pos.y == ymax) and sq.drawn())
        {
          if(debug)
          {
            std::cerr << "drawer is " << int(sq.uDrawer) << "\n";
            std::cerr << "pos " << i << "," << j << ", ";
            std::cerr << "Painting from ability\n";
            std::cerr << "plId = " << plId << ", uid = " << int(sq.uDrawer) << ", p = " << int(pos.x) << "," << int(pos.y) << "\n";
            std::cerr << int(sq.plDrawer) << "\n";
          }          

          Position drawingOut(-1,-1);
          for(int i = 0; i < NORMAL_DIRS.size(); ++i)
          {
            Position aux2 = pos+NORMAL_DIRS[i];
            if(not info.posOk(aux2)) continue;
            Square sqaux2 = info.square_map[aux2];
            if(sqaux2.uDrawer == sq.uDrawer)
            {
              drawingOut = aux2;
              break;
            }
          }
          if(drawingOut == Position(-1,-1))
          {
            if(debug) std::cerr << "didn't find out drawing position" << std::endl;
            info.square_map[pos].uDrawer = -1;
            info.square_map[pos].plDrawer = -1;
            continue;
          }

          paint(plId,sq.uDrawer,pos,drawingOut);
        }
        else if(sq.drawn()) //Specific case, DON'T TOUCH
        {
          sq.uDrawer = -1;
          sq.plDrawer = -1;
          info.square_map[pos] = sq;
        }
      }
    }
  }
}

void Board::resolveAbilities()
{
  if(abilityUnits.size() == 0) return;
  if(abilityUnits.size() == 1)
  {
    if(killedUnits[abilityUnits[0]])
    {
      abilityUnits.clear();
      return;
    }

    Unit u = GameInfo::unit(abilityUnits[0]);
    if(debug) std::cerr << "unit " << u.id() << " by player " << u.player() << " using ability\n";

    useAbility(u.player(),u.position());
    abilityUnits.clear();
    return;
  }

  if(debug)
  {
    std::cerr << "ability conflicts can occur\n";
    std::cerr<< "units that can use the ability:\n";
    for(int i = 0; i < abilityUnits.size(); ++i)
    {
      std::cerr << abilityUnits[i] << " ";
    }
    std::cerr << "\n";
  }
  
  //Conflicts can occur, organize data
  vector<pair<int,Position>> unitsMap(info.numPlayers,make_pair(-1,Position(-1,-1)));
  for(int i = 0; i < abilityUnits.size(); ++i)
  {
    if(killedUnits[abilityUnits[i]]) continue;
    Unit u = GameInfo::unit(abilityUnits[i]);
    int player = u.player();
    unitsMap[player] = make_pair(abilityUnits[i], u.position());
  }

  vector<bool> canUse(info.numPlayers,true);

  //invalidates overlapping abilities
  for(int i = 0; i < unitsMap.size()-1; ++i)
  {
    if(unitsMap[i].first == -1)
    {
      if(canUse[i]) canUse[i] = false;
      continue;
    }
    Position p = unitsMap[i].second;
    for(int j = i+1; j < unitsMap.size(); ++j)
    {
      if(unitsMap[j].first == -1)
      {
        if(canUse[j]) canUse[j] = false;
        continue;
      }
      Position p2 = unitsMap[j].second;
      if(abs(p2.x-p.x) < info.abilitySize and abs(p2.y-p.y) < info.abilitySize)
      {
        if(canUse[i]) canUse[i] = false;
        if(canUse[j]) canUse[j] = false;
      }
    }
  }

  //uses abilities
  for(int i = 0; i < info.numPlayers; ++i)
  {
    if(canUse[i])
    {
      Unit u = info.unit(unitsMap[i].first);
      useAbility(u.player(),unitsMap[i].second);
    }
  }

  //End of function
  abilityUnits.clear();
}

void Board::giveBoardPoints()
{
  for(int i = 0; i < info.numPlayers; ++i)
  {
    info.playerPoints[i] += info.pointsPerSquare * info.player_squares[i].size();
  }
}

void Board::moveBubbles()
{
  for(int i = 0; i < info.bubblesVector.size(); ++i)
  {
    if(info.bubblesVector[i].player() == -1) continue;
    if(++info.bubbleMovementCounters[i] < info.roundsPerBubbleMove) continue;

    info.bubbleMovementCounters[i] = 0;

    Bubble b = info.bubblesVector[i];

    vector<Position> opts;
    opts.reserve(4);
    const Direction DIRS_STRAIGHT[4] = {Direction::left, Direction::right, Direction::up, Direction::down};
    for(int j = 0; j < 4; ++j)
    {
      Position aux = b.position()+DIRS_STRAIGHT[j];
      if(info.posOk(aux))
      {
        Square sq = info.square(aux);
        if(not sq.empty()) continue;
        bool ability = info.square(b.position()).ability();
        if(ability and not sq.ability()) continue;
        if(not ability and sq.ability()) continue;
        opts.emplace_back(aux);
      }
    }

    if(opts.size() == 0) continue;

    Position movPos = opts[GameInfo::randomNumber(0,opts.size()-1)];

    info.square_map[b.position()].bb = nullptr;
    b.p = movPos;
    info.bubblesVector[i] = b;
    info.square_map[movPos].bb = &info.bubblesVector[i];
  }
}

void Board::getPlayerSquares()
{
  for(int i = 0; i < info.numPlayers; ++i)
  {
    info.player_squares[i].clear();
  }

  for(int i = 0; i < info.rows(); ++i){
    for(int j = 0; j < info.cols(); ++j)
    {
      Position ij(i,j);
      int painter = info.square_map[ij].painter();
      if(painter >= 0)
      {
        if(painter >= info.numPlayers)
        {
          std::cerr << "AT getPlayerSquares(), painter is " << painter << "\n";
          std::cerr << "position " <<  i << "," << j << "\n";
          exit(1);
        }
        info.player_squares[painter].push_back(Position(i,j));

        //Decrement ability counter of all squares
        if(info.square_map[ij].isAbility)
        {
          if(--info.square_map[ij].counter == 0)
          {
            info.square_map[ij].isAbility = false;
          }
        }
      }
    }
  }
}

void Board::respawn()
{
  //Units
  vector<int> nUnits = GameInfo::numberOfUnits();
  for(int i = 0; i < info.respawnCounters.size(); ++i)
  {
    if(nUnits[i] >= info.unitsMax) continue;
    if(++info.respawnCounters[i] < info.roundsPerRespawn) continue;
    
    Position p;
    if(info.freeSquare(i,p)) //found free square
    {
      info.respawnCounters[i] = 0;
      info.spawnUnit(i,p);
    }
    else
    {
      info.respawnCounters[i]--;
    }
  }

  //Bubbles
  for(int i = 0; i < info.bubbleCounters.size(); ++i)
  {
    if(++info.bubbleCounters[i] < info.roundsPerBubble) continue;

    Position p;
    if(info.freeSquare(i,p)) //found free square
    {
      info.bubbleCounters[i] = 0;
      info.spawnBubble(i,p);
    }
    else
    {
      info.bubbleCounters[i]--;
    }
  }

  //Bonus
  for(int i = 0; i < info.bonusCounters.size(); ++i)
  {
    if(++info.bonusCounters[i] < info.roundsPerBonus) continue;
    if(info.currentBonuses == info.bonusMax) continue;
    bool foundPos = false;
    int x = -1;
    int y = -1;
    while(not foundPos) //Looks for an empty random position
    { 
      x = GameInfo::randomNumber(0,info.rows()-1);
      y = GameInfo::randomNumber(0,info.cols()-1);
      if(info.square_map[Position(x,y)].empty())
      {
        foundPos = true;
      }
    }
    if(x == -1 or y == -1)
    {
      std::cerr << "error: respawn() with bonus is not working properly\n";
      exit(1);
    }

    //find free bonus
    int index = -1;
    for(int j = 0; j < info.bonusVector.size(); ++j)
    {
      if(info.bonusVector[j].p == Position(-1,-1))
      {
        index = j;
        break;
      }
    }
    if(index == -1)
    {
      std::cerr << " at least one bonus should be free (position -1,-1), but it isn't\n";
      exit(1);
    }
    Position xy(x,y);
    info.bonusVector[index].p = xy;
    info.square_map[xy].b = &info.bonusVector[index];
    info.bonusCounters[index] = 0;
    ++info.currentBonuses;
 }
}

void Board::computeEnergies()
{
  for(Unit& u : info.unitsVector)
  {
    if(u.player() < 0) continue;
    else if(info.square(u.position()).painter() < 0) continue;
    else if(u.player() == info.square(u.position()).painter())
    {
      if(u.energ < info.energyMax) u.energ++;
    }
    else
    {
      //Might want to change this when the unit is upgraded
      if(not u.upgraded() and u.energ > info.energyMin) u.energ--;
    }
  }
}

void Board::executeRound(const vector<Player*>& pl)
{
  //if(info.round() == 81) exit(1);

  info.old_square_map = info.square_map;
  info.old_playerPoints = info.playerPoints;
  if(debugBasic) std::cerr << "Executing round " << info.round() << "\n";

  killedUnits = vector<bool>(info.unitsMax*info.numPlayers,false);

  //this has to happen in random order
  /*
    idea:
      guarda por qué orden vamos para cada player
      guarda cuántos tienen órdenes
      para elegir de quién es la orden, randomNumber(0,jugadoresConÓrdenes-1)
      vas al vector<int> de por qué orden vamos y vas contando para ver quién es
      ejecutas su orden
      si no le quedan, decrementa el número  
  */

  int orderable = 0;
  std::vector<int> playerOrderIndex(info.numPlayers,-1);
  for(int i = 0; i < pl.size(); ++i)
  {
    if(pl[i]->index >= 0)
    {
      orderable++;
      playerOrderIndex[i] = 0;
    }
  }

  while(orderable > 0)
  {
    int random = GameInfo::randomNumber(0,orderable-1);
    int who = -1;

    for(who = 0; who < playerOrderIndex.size(); ++who)
    {
      if(playerOrderIndex[who] != -1 and playerOrderIndex[who] <= pl[who]->index)
      {
        random--;
      }
      if(random == -1) break;
    }

    if(executeOrder(who,pl[who]->orderList[playerOrderIndex[who]]))
    {
      if(debugOrders) std::cerr << "order executed succesfully\n";
    }
    else
    {
      if(debugOrders) std::cerr << "order didn't execute\n";
    }
    playerOrderIndex[who]++;
    if(playerOrderIndex[who] > pl[who]->index)
    {
      --orderable;
    }
  }

  /*
  for(int i = 0; i < pl.size(); ++i){
    for(int j = 0; j <= pl[i]->index; ++j){
      if(executeOrder(i,pl[i]->orderList[j])){
        if(debugOrders) std::cerr << "order executed succesfully" << std::endl;
      }
      else{
        if(debugOrders) std::cerr << "order didn't execute" << std::endl;
      }
    }
  }*/
  if(debugBasic) std::cerr << "\texecuting free attacks...\n";
  performFreeAttacks();
  if(debugBasic) std::cerr << "\tpopping bubbles...\n";
  popBubbles();
  if(debugBasic) std::cerr << "\tresolving abilities...\n";
  resolveAbilities();
  
  if(debugBasic) std::cerr << "\tmoving bubbles...\n";
  moveBubbles();
  if(debugBasic) std::cerr << "\tgetting player squares...\n";
  getPlayerSquares();
  if(debugBasic) std::cerr << "\trespawning...\n";
  respawn();
  if(debugBasic) std::cerr << "\tcomputing energies...\n";
  computeEnergies();
  if(debugBasic) std::cerr << "\tgiving board points...\n";
  giveBoardPoints();
}

void Board::printRound(bool new_viewer)
{
  //std::cerr << std::endl << "Printing round " << info.round() << std::endl << std::endl;

  if(info.round()==0)
  {
    debugBasic = false;
    debug = false;
    debugOrders = false;
    debugPaint = false;
    debugDrawErase = false;
    debugFlood = false;
  }

  //VIEWER FORMAT
  if(view)
  {
    cout << info.round() << " ";
    if (new_viewer)
    {
      /*
        New viewer format: incremental
        - On player points, prints increases to the total amount
        - On each round, prints the incremental changes to the board:
          Prints an offset (cells to skip if interpreting the board by rows) and 
          then information about the first affected cell after the skip. The information about
          the cell is complete (both square and unit info).

          This produces about 13x smaller files
      */
      for(int i = 0; i < info.numPlayers; ++i)
      {
        cout << info.playerPoints[i]-info.old_playerPoints[i] << " ";
      }

      int jump = 0;

      for(int i = 0; i < info.boardHeight; ++i){
        for(int j = 0; j < info.boardWidth; ++j)
        {
          Square const& sq = info.square(Position(i,j));
          Square const& old_sq = info.old_square_map[Position(i,j)];

          if (sq != old_sq)
          {
            cout << jump;
            jump = 0;
            cout << sqcode(sq.plDrawer,sq.painter(),sq.ability());
            if(sq.hasUnit()){
              cout << ucode(true,sq.unit().player(),sq.unit().upgraded());
            }
            else if(sq.hasBubble()){
              cout << ucode(false,sq.bubble().player());
            }
            else if(sq.hasBonus()){
              cout << ucode(true);
            } 
            else cout << ucode(false);
          }

          ++jump;
        }
      }
    }
    else
    {
      // Using old viewer format

      /*
        Each round, prints how many points each player has
        Also prints the complete state of the board      
      */
      for(int i = 0; i < info.numPlayers; ++i)
      {
        cout << info.playerPoints[i] << " ";
      }

      for(int i = 0; i < info.boardHeight; ++i)
      {
        for(int j = 0; j < info.boardWidth; ++j)
        {
          Square sq = info.square(Position(i,j));
          //Prints painter id, drawer id and unit's player id
          cout << sqcode(sq.plDrawer,sq.painter(),sq.ability());
          if(sq.hasUnit()){
            cout << ucode(true,sq.unit().player(),sq.unit().upgraded());
          }
          else if(sq.hasBubble()){
            cout << ucode(false,sq.bubble().player());
          }
          else if(sq.hasBonus()){
            cout << ucode(true);
          } 
          else cout << ucode(false);
        }
      }
    }
    
    cout << "\n";
  }

  //CONSOLE FORMAT
  if(debug)
  {
    printMatrix(info.square_map);
  }
}

void Board::printSettings()
{
  info.printSettings();
}

void Board::spawnPlayer(int i, int j, int plId)
{
  Position p(i,j);
  info.square_map[p].plPainter = plId;

  Unit u = info.unitsVector[plId];
  u.pl = plId;
  u.p = p;
  info.unitsVector[plId] = u;

  //Center square
  info.square_map[p].u = &info.unitsVector[plId];
  info.square_map[p].isBorder = false;
  info.square_map[p].plPainter = plId;

  const vector<Direction> DIRS = {DL,Direction::left,UL,down,up,DR,Direction::right,UR};
  for(int k = 0; k < DIRS.size(); ++k)
  {
    Position aux = p+DIRS[k];
    info.square_map[aux].isBorder = true;
    info.square_map[aux].plPainter = plId;
  }
}

void Board::spawnPlayers()
{
  int playersToSpawn = info.numPlayers;

  /*if(playersToSpawn < 2)
  {
    std::cerr << "can't play alone, sorry\n";
    exit(1);
  }*/

  /* order is:
      bot-left
      up-right
      bot-right
      up-left
      at least 9x9
  */

  int cols = info.cols();
  int rows = info.rows();
  int i,j;

  //bottom left
  i = rows-2;
  j = 1;
  spawnPlayer(i,j,0);

  if(playersToSpawn >= 2)
  {
    //up right
    i = 1;
    j = cols-2;
    spawnPlayer(i,j,1);
  }

  if(playersToSpawn >= 3)
  {
    //bot-right
    i = rows-2;
    j = cols-2;
    spawnPlayer(i,j,2);
  }

  if(playersToSpawn >= 4)
  {
    //up left
    i = 1;
    j = 1;
    spawnPlayer(i,j,3);
  }
}

void Board::printPainters()
{
  for(int i = 0; i < info.rows(); ++i){
    for(int j = 0; j < info.cols(); ++j)
    {
      std::cerr << info.square_map[Position(i,j)].painter() << " ";
    }
    std::cerr << "\n";
  }
  std::cerr << "\n";
}

void Board::printStartSquares(const vector<pair<Position,Position>>& sqs)
{
  for(int i = 0; i < sqs.size(); ++i)
  {
    if(debug) std::cerr << "unit " << i << " start: " << int(sqs[i].first.x) << "," << int(sqs[i].first.y) << "\n";
  }
}
