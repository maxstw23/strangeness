#include "NpartNormalizer.h"
#include "TString.h"

const float NpartNormalizer::ratio_ox[] =
{
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
    0.0, 0.0, 1.5892029273382569, 0.4674126256877227, 0.6621678863909404, 0.3178405854676514, 0.22072262879698015, 1.049473631261113, 0.6320693461004431, 0.8590286093720308, 
    0.5036206459874758, 0.7904412465818557, 0.6063224019965238, 0.7532919074539138, 0.7542574599949785, 0.7685014155924053, 0.7956914519594839, 0.5984446169893018, 0.6685967008219205, 0.7301743179662262, 
    0.7192696698451136, 0.7912201808450045, 0.8190286334383422, 0.8667940609456612, 0.8039077853218694, 0.9445532708014055, 0.7503691754330736, 0.8682106253866962, 0.7969509653358025, 0.7721882059976082, 
    0.812621657248932, 0.8017736938027825, 0.8519406874637606, 0.8157622127981018, 0.8387671415731145, 0.8635376618693645, 0.8898252476905187, 0.8695187531223864, 0.8893860163231249, 0.9060726614078762, 
    0.9012082724028879, 0.9041790680652055, 0.8865220218728429, 0.9348507851423492, 0.9090526374580088, 0.9427034826703032, 0.9301304925328766, 0.934086120436016, 0.9307646549472385, 0.9584187035339616, 
    0.9595663534039773, 0.9618151918833627, 0.9676800862142984, 0.9796063155090969, 0.98982471526682, 0.9906386683956582, 1.018360734182821, 1.0024395688700176, 1.0263558623733111, 1.0272881973290031, 
    1.0398354060046844, 1.0417193422936852, 1.0506497329954285, 1.0523889952692225, 1.060901583305789, 1.0786124134821615, 1.0918499798379775, 1.0813971680370627, 1.0870000986020891, 1.0973685878728778, 
    1.1094560176969857, 1.1090305431091578, 1.1091524973022846, 1.121806107301923, 1.1400724678643195, 1.099198691408961, 1.2224637902601976, 0.0, 0.0, 0.0
};


const float NpartNormalizer::ratio_oxb[] = 
{
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
    0.0, 3.571068281438364, 0.0, 0.0, 0.0, 0.7142136562876727, 1.020305223268104, 0.6157014278342007, 0.38398583671380265, 0.9156585337021447, 
    0.7388417134010409, 0.892767070359591, 0.5493951202212868, 0.9452827803807435, 0.868230131850765, 1.0041668278209812, 0.7662240135726008, 0.7984095751183334, 0.7064564870184507, 0.7963920860350586, 
    0.9806560565607942, 0.6666185270448974, 0.7376225026528543, 0.7386660496555818, 0.8641731072519885, 0.7071751059374968, 0.7873621388662112, 0.8087211907931515, 0.7959320164670057, 0.7981271522053096, 
    0.907077763226021, 0.8883316591220617, 0.8272369292939524, 0.8482502962666552, 0.8512967569155615, 0.8628132020072442, 0.8217150995921083, 0.8618847471233329, 0.8640003533911907, 0.8716127216618083, 
    0.8986789005766761, 0.9097517851588796, 0.9016339679449056, 0.8941264357983689, 0.8933712692362371, 0.9249441529692414, 0.9448443375327005, 0.9353312912502618, 0.9480377109891635, 0.944121369615169, 
    0.9515190244189797, 0.9421693771273024, 0.9991240573445176, 0.9759307059074399, 0.997252701850723, 0.9838805355052447, 1.0189639848363339, 1.024476325738933, 1.0159137615253984, 1.039769960478326, 
    1.0206877561573164, 1.0371858460414962, 1.0607818473825135, 1.0572738101237675, 1.07230065424195, 1.0791436500941378, 1.0605727589296046, 1.0840817957934143, 1.0998850714380732, 1.1220921934631378, 
    1.1160799403735746, 1.0931675930898428, 1.1291751629132067, 1.0905203799236005, 1.2195750801070186, 0.9789414980004781, 0.6648265417571423, 0.0, 0.0, 0.0
};

const float NpartNormalizer::ratio_obx[] = 
{
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
    0.0, 0.0, 7.64640612899984, 0.0, 0.764640612899984, 0.0, 0.8496006809999822, 0.95580076612498, 0.5461718663571314, 1.146960919349976, 
    0.3729954209268215, 0.7871300426911599, 0.3861821277272646, 0.46951616581577965, 0.5664004539999882, 0.6086691445970022, 0.897756185186216, 0.5664004539999882, 0.5086665021437662, 0.7528314914652352, 
    0.7784518554975206, 0.8206875688193546, 0.8896761848207361, 0.8301441768549445, 0.7458519753529256, 0.9783280569090703, 0.7333145542709343, 0.8741342449687317, 0.8116297008435585, 0.7596344261374735, 
    0.8108441178113037, 0.8131179294561304, 0.8771438452219268, 0.8007385166613703, 0.8289200578790727, 0.8855506286657593, 0.9095256296547561, 0.8941917142213532, 0.8774263279033289, 0.9098685818398174, 
    0.8897242103584634, 0.9214761183364343, 0.9122328039650359, 0.9166297489079288, 0.9094664026242061, 0.9201135283137071, 0.9518080369061235, 0.9300016487685578, 0.9211042359291342, 0.9482902032206839, 
    0.9730028712857317, 0.9685547167886528, 0.9476687709508137, 0.9730234957659338, 0.9902631813753054, 0.9845906154523553, 1.0187236938088917, 1.0045879754322733, 1.0160504362034584, 1.0096371327529547, 
    1.0523068727855291, 1.0308113684245221, 1.0457758382447697, 1.0611679412723347, 1.0602880769596392, 1.052716322693269, 1.0864883394214842, 1.077499675173007, 1.0678921497720308, 1.0862075322313007, 
    1.136298477094782, 1.1179328539871933, 1.113074819056515, 1.109533691312055, 1.242747432413041, 1.0573186831683143, 1.1099621800161057, 0.0, 0.0, 0.0
};

const float NpartNormalizer::ratio_obxb[] = 
{
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
    0.0, 18.156946208956008, 0.0, 0.0, 0.0, 0.0, 2.0174384676617785, 1.4525556967164805, 0.0, 0.8253157367707276, 
    0.8857046931198052, 1.022926546983437, 0.1762810311549127, 0.7826269917653451, 0.7610696015730063, 0.6052315402985335, 0.8974916377050497, 0.7523596495423759, 0.5650501932247719, 0.7648530454322127, 
    1.0596325074627322, 0.6215091611867581, 0.6718246893284597, 0.7222333416450281, 0.9733888246402433, 0.6261015934122761, 0.8392544025472999, 0.7938198046153423, 0.8646164861407623, 0.728625096992848, 
    0.8753801590103281, 1.003262483725158, 0.8536225338324641, 0.8880970915462494, 0.8460471041852146, 0.8677225545103077, 0.8211510472487958, 0.8198233651756734, 0.8678052719168345, 0.9208095699989138, 
    0.8950374985823405, 0.9043421084769594, 0.9001498519552255, 0.8875440159649423, 0.8571103202274403, 0.9428147279398821, 0.9494253693167112, 0.9174485158461907, 0.9470390268853951, 0.9509046532998856, 
    0.9553921800454627, 0.9644872999237124, 1.0331565773153522, 0.9553112406022134, 0.9754712511484842, 1.009487188351675, 1.0195137392895226, 1.0039126218836887, 0.984853325713581, 1.0368462407329728, 
    1.0161601289763, 1.001892403517876, 1.0473907874862625, 1.1073439659071014, 1.054408858224431, 1.0593886741505782, 1.0676099777657073, 1.096572881669783, 1.079901146330058, 1.1121123114169484, 
    1.1218111003257525, 1.111445787975288, 1.10609168954899, 1.0561547558750612, 0.9869838536699886, 1.0124691194585134, 1.0839967885943884, 0.0, 0.0, 0.0
};

const float NpartNormalizer::ratio_x[] = 
{
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 7.938763600162205, 0.0, 0.0, 
    0.0, 0.0, 1.3231272666937008, 0.37803636191248596, 0.8505818143030934, 0.2737504689711105, 0.17258181739483053, 1.2655999942287575, 0.49617272501013787, 0.716279422571026, 
    0.5475009379422211, 0.7185949810491651, 0.5707607817110082, 0.7322160602091354, 0.7000410679317517, 0.7250012420239457, 0.8327374405764552, 0.6241607424155797, 0.6642861174346976, 0.739016828570211, 
    0.746751279284664, 0.7622363601605016, 0.8216717853976239, 0.8521143941486846, 0.7933720980894687, 0.9148158225208192, 0.7655596084182924, 0.8747712354420503, 0.8068565750265786, 0.7800154581467145, 
    0.8172860612686388, 0.8052439203343736, 0.8589106832677433, 0.8246932452028144, 0.8386784020104087, 0.8647068799474897, 0.9055515757527907, 0.8704026194595662, 0.8881255846883963, 0.9053185570651356, 
    0.9055035172755003, 0.90791827762759, 0.8878518997967486, 0.9369918976198617, 0.9065242094880466, 0.9390991086991277, 0.9311772947553295, 0.9363263294051586, 0.9361899463088337, 0.9548877615877397, 
    0.9616256373883222, 0.9683572516590596, 0.9702756843245292, 0.9763595615183572, 0.9910818240199818, 0.9901914620994748, 1.0213358087740374, 1.0032054364513843, 1.0282309397683407, 1.0300259749333889, 
    1.0409484541942775, 1.0414868336419025, 1.052345804201895, 1.0526207564088412, 1.0607510687679222, 1.078684852124406, 1.09423276248809, 1.0832713864599102, 1.0907002478950747, 1.0972753485616455, 
    1.107675354150681, 1.1068931154448762, 1.1099212086756085, 1.125373631019648, 1.133395809582906, 1.1120447066375074, 1.1279117683290565, 0.0, 0.0, 0.0
};

const float NpartNormalizer::ratio_xb[] =
{
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
    0.0, 3.612328710714989, 0.0, 0.8600782644559496, 0.6020547851191648, 0.6228152949508601, 0.8027397134922197, 0.7037003981912314, 0.30874604365085373, 0.9161703251813377, 
    0.6020547851191648, 0.9783390258186427, 0.6267447289884049, 0.8841363977274448, 0.8135875474583307, 0.9269171911658695, 0.7542224780613712, 0.7070941306080403, 0.7253194580434967, 0.7263921863937749, 
    0.9606412615307992, 0.7008810856984782, 0.7214202056131236, 0.7613192776942926, 0.8710114627329321, 0.7254478201388254, 0.7826550667835668, 0.8259087714368168, 0.8127307295529175, 0.7827348576352098, 
    0.884864381587245, 0.8797593346516083, 0.823645705403062, 0.8389914185812705, 0.8465174818488315, 0.8574354331641191, 0.822413864707596, 0.8638995764750891, 0.8689828506287578, 0.8696303193953573, 
    0.8941972963226285, 0.909586172959718, 0.8950167058811036, 0.8947507106430356, 0.8913746081824345, 0.9213220212384552, 0.9450252890336476, 0.9379785355166903, 0.9506625946472123, 0.9487808307800478, 
    0.9540582352303529, 0.9456037495572541, 0.9988603449547071, 0.9766367501810259, 0.9943100541892919, 0.9928744739399863, 1.0234123553424135, 1.0215063698471518, 1.0183472422072573, 1.0427266901420966, 
    1.0286469733522776, 1.0380569483532693, 1.0644768663092743, 1.0555643333867533, 1.0743335347047347, 1.0777961073864801, 1.0617383712373576, 1.0896231007880828, 1.1063827803077453, 1.113922110647276, 
    1.1211542493403177, 1.1000904900427937, 1.1333022321354684, 1.095238431732392, 1.2128315855278398, 0.9745491126029646, 0.6258985389852704, 0.0, 0.0, 0.0
};

const float NpartNormalizer::ratio_o[] = 
{
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
    0.0, 0.0, 0.0, 0.0, 0.0, 0.1145070473777249, 0.0, 0.1145070473777249, 0.0, 0.4580281895108996, 
    0.4580281895108996, 0.0, 0.6870422842663493, 0.515281713199762, 0.5954366463641695, 0.7442958079552118, 0.4236760752975821, 0.3435211421331747, 0.4532570625368277, 0.6401984921572801, 
    0.494016118686756, 0.5592204639377262, 0.5114648116205046, 0.4455817713176686, 0.6761368511827565, 0.5932649609828677, 0.629004465732434, 0.6099131849963793, 0.6393310145256307, 0.6209509683302128, 
    0.6476009679473552, 0.670943787809617, 0.6431795478859317, 0.6410287161483066, 0.6520704154151628, 0.6661903245461602, 0.6459722928926137, 0.6992857301013522, 0.660830683470828, 0.6910600754024099, 
    0.6836728569030628, 0.6985907786745136, 0.7281333299760593, 0.7406295085728073, 0.7438122477889207, 0.7678981622724644, 0.7593786568235023, 0.7564577649084302, 0.7743014883297644, 0.7786145219602518, 
    0.8002406796740431, 0.8202662150163964, 0.8290568571902678, 0.8294834832939925, 0.8369250846683056, 0.8655403348794544, 0.8677464347329347, 0.8978924496446273, 0.9183993393378829, 0.9233440816093976, 
    0.9267919966528381, 0.9473550919246201, 0.9501997339653757, 0.9604816068086657, 1.009550654874587, 1.0178268310848473, 1.0041981611795694, 1.0320772483818057, 1.0512755727387002, 1.0772551725332655, 
    1.08162677010994, 1.1037820574098582, 1.1304546705247698, 1.1433152506187139, 1.1660311125613168, 1.1807938462389227, 1.2046770022734439, 1.229437428878175, 1.2508385883495927, 1.2299199533415623, 
    1.2625433568415592, 1.3047931244784858, 1.2808377872804797, 1.314396796387677, 1.2849950047657244, 1.5799659264441637, 1.594919588475454, 0.0, 0.0, 0.0
};

const float NpartNormalizer::ratio_ob[] = 
{
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
    0.0, 0.0, 0.0, 0.0, 0.0, 1.848367212433306, 0.0, 1.848367212433306, 1.848367212433306, 0.4620918031083265, 
    0.4620918031083265, 0.924183606216653, 0.4349099323372484, 0.1848367212433306, 1.0166019668383182, 0.5625465429144845, 0.49955870606305564, 0.9416210327490426, 0.6230451277865076, 0.8588372906255765, 
    0.5558027282142108, 0.7788064097331345, 0.8581704914868921, 0.6797946282809423, 0.7921573767571312, 0.8021913701960548, 0.7591508193922506, 0.8256332216674838, 0.848234999611591, 0.7819486571558001, 
    0.718988057705941, 0.7176313270851661, 0.778683180870835, 0.8066163427880905, 0.7321938379808366, 0.8179696382630878, 0.8023338406911307, 0.8107833132084433, 0.8241626754603931, 0.7958076002695015, 
    0.8316126067015096, 0.8413852582974178, 0.8420997390366648, 0.8347689730383397, 0.8420317068230231, 0.8588652321941957, 0.8733107492185423, 0.8888984846180095, 0.8841093995424701, 0.8864276192300269, 
    0.8946743774571022, 0.8917610432000789, 0.8996031180546392, 0.9062796438858503, 0.9137315947501999, 0.9209340513706932, 0.9346156509901534, 0.9218561330778497, 0.9376542966950531, 0.9588385043569941, 
    0.9615197897481198, 0.9635037253618857, 0.9674275372842708, 0.971333372909017, 0.9869741593483484, 0.992920139166649, 1.0125570480387154, 1.01898212207994, 1.015184143446347, 1.0268500603037525, 
    1.027853353781847, 1.0457036808552578, 1.0506118023157953, 1.0668702432672055, 1.0788659248502979, 1.0845641945896505, 1.0926680843009247, 1.1009686267368064, 1.1130066043231859, 1.108658756757662, 
    1.1362743011759504, 1.131955370923132, 1.1582056411839547, 1.1415786476260816, 1.206650881858371, 1.1704265068696964, 0.9509715368316285, 0.0, 0.0, 0.0
};

float NpartNormalizer::weight(int npp, TString compare_case)
{
    if (npp < lowest_npp || npp >= lowest_npp + 100) return 0;
    if (compare_case == "ox")   return ratio_ox[npp-lowest_npp];
    if (compare_case == "oxb")  return ratio_oxb[npp-lowest_npp];
    if (compare_case == "obx")  return ratio_obx[npp-lowest_npp];
    if (compare_case == "obxb") return ratio_obxb[npp-lowest_npp];

    if (compare_case == "x")  return ratio_x[npp-lowest_npp];
    if (compare_case == "xb") return ratio_xb[npp-lowest_npp];
    if (compare_case == "o")  return ratio_o[npp-lowest_npp];
    if (compare_case == "ob") return ratio_ob[npp-lowest_npp];

    return 0;
}

