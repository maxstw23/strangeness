#include "NpartNormalizer.h"
#include "TString.h"

const float NpartNormalizer::ratio_ox[] =
{
    0.812621657248932, 0.8017736938027825, 0.8519406874637606, 0.8157622127981018, 0.8387671415731145, 0.8635376618693645, 0.8898252476905187, 0.8695187531223864, 0.8893860163231249, 0.9060726614078762, 
    0.9012082724028879, 0.9041790680652055, 0.8865220218728429, 0.9348507851423492, 0.9090526374580088, 0.9427034826703032, 0.9301304925328766, 0.934086120436016, 0.9307646549472385, 0.9584187035339616, 
    0.9595663534039773, 0.9618151918833627, 0.9676800862142984, 0.9796063155090969, 0.98982471526682, 0.9906386683956582, 1.018360734182821, 1.0024395688700176, 1.0263558623733111, 1.0272881973290031, 
    1.0398354060046844, 1.0417193422936852, 1.0506497329954285, 1.0523889952692225, 1.060901583305789, 1.0786124134821615, 1.0918499798379775, 1.0813971680370627, 1.0870000986020891, 1.0973685878728778 
};


const float NpartNormalizer::ratio_oxb[] = 
{
    0.907077763226021, 0.8883316591220617, 0.8272369292939524, 0.8482502962666552, 0.8512967569155615, 0.8628132020072442, 0.8217150995921083, 0.8618847471233329, 0.8640003533911907, 0.8716127216618083, 
    0.8986789005766761, 0.9097517851588796, 0.9016339679449056, 0.8941264357983689, 0.8933712692362371, 0.9249441529692414, 0.9448443375327005, 0.9353312912502618, 0.9480377109891635, 0.944121369615169, 
    0.9515190244189797, 0.9421693771273024, 0.9991240573445176, 0.9759307059074399, 0.997252701850723, 0.9838805355052447, 1.0189639848363339, 1.024476325738933, 1.0159137615253984, 1.039769960478326, 
    1.0206877561573164, 1.0371858460414962, 1.0607818473825135, 1.0572738101237675, 1.07230065424195, 1.0791436500941378, 1.0605727589296046, 1.0840817957934143, 1.0998850714380732, 1.1220921934631378 
};

const float NpartNormalizer::ratio_obx[] = 
{
    0.8108441178113037, 0.8131179294561304, 0.8771438452219268, 0.8007385166613703, 0.8289200578790727, 0.8855506286657593, 0.9095256296547561, 0.8941917142213532, 0.8774263279033289, 0.9098685818398174, 
    0.8897242103584634, 0.9214761183364343, 0.9122328039650359, 0.9166297489079288, 0.9094664026242061, 0.9201135283137071, 0.9518080369061235, 0.9300016487685578, 0.9211042359291342, 0.9482902032206839, 
    0.9730028712857317, 0.9685547167886528, 0.9476687709508137, 0.9730234957659338, 0.9902631813753054, 0.9845906154523553, 1.0187236938088917, 1.0045879754322733, 1.0160504362034584, 1.0096371327529547, 
    1.0523068727855291, 1.0308113684245221, 1.0457758382447697, 1.0611679412723347, 1.0602880769596392, 1.052716322693269, 1.0864883394214842, 1.077499675173007, 1.0678921497720308, 1.0862075322313007
};

const float NpartNormalizer::ratio_obxb[] = 
{
    0.8753801590103281, 1.003262483725158, 0.8536225338324641, 0.8880970915462494, 0.8460471041852146, 0.8677225545103077, 0.8211510472487958, 0.8198233651756734, 0.8678052719168345, 0.9208095699989138, 
    0.8950374985823405, 0.9043421084769594, 0.9001498519552255, 0.8875440159649423, 0.8571103202274403, 0.9428147279398821, 0.9494253693167112, 0.9174485158461907, 0.9470390268853951, 0.9509046532998856, 
    0.9553921800454627, 0.9644872999237124, 1.0331565773153522, 0.9553112406022134, 0.9754712511484842, 1.009487188351675, 1.0195137392895226, 1.0039126218836887, 0.984853325713581, 1.0368462407329728, 
    1.0161601289763, 1.001892403517876, 1.0473907874862625, 1.1073439659071014, 1.054408858224431, 1.0593886741505782, 1.0676099777657073, 1.096572881669783, 1.079901146330058, 1.1121123114169484
};

const float NpartNormalizer::ratio_x[] = 
{
    0.8172860612686388, 0.8052439203343736, 0.8589106832677433, 0.8246932452028144, 0.8386784020104087, 0.8647068799474897, 0.9055515757527907, 0.8704026194595662, 0.8881255846883963, 0.9053185570651356, 
    0.9055035172755003, 0.90791827762759, 0.8878518997967486, 0.9369918976198617, 0.9065242094880466, 0.9390991086991277, 0.9311772947553295, 0.9363263294051586, 0.9361899463088337, 0.9548877615877397, 
    0.9616256373883222, 0.9683572516590596, 0.9702756843245292, 0.9763595615183572, 0.9910818240199818, 0.9901914620994748, 1.0213358087740374, 1.0032054364513843, 1.0282309397683407, 1.0300259749333889, 
    1.0409484541942775, 1.0414868336419025, 1.052345804201895, 1.0526207564088412, 1.0607510687679222, 1.078684852124406, 1.09423276248809, 1.0832713864599102, 1.0907002478950747, 1.0972753485616455
};

const float NpartNormalizer::ratio_xb[] =
{
    0.884864381587245, 0.8797593346516083, 0.823645705403062, 0.8389914185812705, 0.8465174818488315, 0.8574354331641191, 0.822413864707596, 0.8638995764750891, 0.8689828506287578, 0.8696303193953573, 
    0.8941972963226285, 0.909586172959718, 0.8950167058811036, 0.8947507106430356, 0.8913746081824345, 0.9213220212384552, 0.9450252890336476, 0.9379785355166903, 0.9506625946472123, 0.9487808307800478, 
    0.9540582352303529, 0.9456037495572541, 0.9988603449547071, 0.9766367501810259, 0.9943100541892919, 0.9928744739399863, 1.0234123553424135, 1.0215063698471518, 1.0183472422072573, 1.0427266901420966, 
    1.0286469733522776, 1.0380569483532693, 1.0644768663092743, 1.0555643333867533, 1.0743335347047347, 1.0777961073864801, 1.0617383712373576, 1.0896231007880828, 1.1063827803077453, 1.113922110647276
};

const float NpartNormalizer::ratio_o[] = 
{
    0.6836728569030628, 0.6985907786745136, 0.7281333299760593, 0.7406295085728073, 0.7438122477889207, 0.7678981622724644, 0.7593786568235023, 0.7564577649084302, 0.7743014883297644, 0.7786145219602518, 
    0.8002406796740431, 0.8202662150163964, 0.8290568571902678, 0.8294834832939925, 0.8369250846683056, 0.8655403348794544, 0.8677464347329347, 0.8978924496446273, 0.9183993393378829, 0.9233440816093976, 
    0.9267919966528381, 0.9473550919246201, 0.9501997339653757, 0.9604816068086657, 1.009550654874587, 1.0178268310848473, 1.0041981611795694, 1.0320772483818057, 1.0512755727387002, 1.0772551725332655, 
    1.08162677010994, 1.1037820574098582, 1.1304546705247698, 1.1433152506187139, 1.1660311125613168, 1.1807938462389227, 1.2046770022734439, 1.229437428878175, 1.2508385883495927, 1.2299199533415623
};

const float NpartNormalizer::ratio_ob[] = 
{
    0.8316126067015096, 0.8413852582974178, 0.8420997390366648, 0.8347689730383397, 0.8420317068230231, 0.8588652321941957, 0.8733107492185423, 0.8888984846180095, 0.8841093995424701, 0.8864276192300269, 
    0.8946743774571022, 0.8917610432000789, 0.8996031180546392, 0.9062796438858503, 0.9137315947501999, 0.9209340513706932, 0.9346156509901534, 0.9218561330778497, 0.9376542966950531, 0.9588385043569941, 
    0.9615197897481198, 0.9635037253618857, 0.9674275372842708, 0.971333372909017, 0.9869741593483484, 0.992920139166649, 1.0125570480387154, 1.01898212207994, 1.015184143446347, 1.0268500603037525, 
    1.027853353781847, 1.0457036808552578, 1.0506118023157953, 1.0668702432672055, 1.0788659248502979, 1.0845641945896505, 1.0926680843009247, 1.1009686267368064, 1.1130066043231859, 1.108658756757662
};

const float NpartNormalizer::ratio_woallx_for_wx[] =
{
    0.8120272606429341, 0.8002532861146286, 0.8508290276198822, 0.8174423391789788, 0.8320721545027603, 0.8584323837813355, 0.8973315506981466, 0.8641535905339328, 0.8817643110622634, 0.899129440798018, 
    0.9000339234759431, 0.9042845180795313, 0.8828564768324837, 0.931764035748507, 0.9010788011308738, 0.934621869348591, 0.9287831874370659, 0.9333382437710998, 0.9339016672916937, 0.9521236226946644, 
    0.9590684804714591, 0.9657700316040844, 0.9703901339660552, 0.9751080105652911, 0.9910082437724653, 0.9903231098954545, 1.0226468529005583, 1.0044635232250045, 1.029173492425015, 1.032462543287026, 
    1.0426356447455218, 1.043206608078994, 1.0557881344881876, 1.0560412833892927, 1.0646710222707134, 1.0832712993843352, 1.0976842636153927, 1.0888678109677332, 1.096562910013524, 1.1039466242510247
};

const float NpartNormalizer::ratio_woallx_for_wxb[] =
{
    0.8664333555438187, 0.8606868512562064, 0.8105165033198052, 0.8222505250681557, 0.831386616835681, 0.844647340828731, 0.8139204519930336, 0.8513406206474284, 0.857857047525033, 0.860421573316577, 
    0.8843084583475364, 0.9008863486920788, 0.8836819716804167, 0.8883920217758731, 0.8817797204043852, 0.9144568533032332, 0.9380435641953642, 0.9313553589096258, 0.9440125454701289, 0.9437959204210827, 
    0.9497252919636928, 0.9424282873130402, 0.995715768560807, 0.9739990333679109, 0.99354086224924, 0.992287780757757, 1.025911898775866, 1.0220011017793909, 1.0215166565659215, 1.0463587708960143, 
    1.0334809156787825, 1.0425154701982453, 1.0705915699445754, 1.0621324229929796, 1.0814150943444443, 1.0874683085220203, 1.0728407630428591, 1.1002821959888571, 1.1173802908966326, 1.1261599549553656
};

const float NpartNormalizer::ratio_woallo_for_wo[] = 
{
    0.6418512825962355, 0.6680190728965442, 0.6889508562805999, 0.7035218551985307, 0.7143774591363875, 0.7419918898819567, 0.7341395726170138, 0.7210126895132106, 0.7360449270359404, 0.7585695980653708, 
    0.7742931154963928, 0.7874684440982225, 0.8160801886371549, 0.8059383562904553, 0.8072298732693953, 0.8402577304097153, 0.8553666275678949, 0.8746619736449264, 0.8997347491344752, 0.9190757304388617, 
    0.9095720277846582, 0.9454828501699779, 0.939942923381336, 0.9561562670238007, 1.0180641822195795, 1.0218667627225608, 1.0134747765258576, 1.045395634232173, 1.0630515899187891, 1.096641873989641, 
    1.0961883593616575, 1.1313942953217433, 1.1468123191987891, 1.1769350910540424, 1.1992786457528632, 1.226798057983709, 1.237023692732749, 1.275977690033549, 1.2937248114176854, 1.2859044504386594
};

const float NpartNormalizer::ratio_woallo_for_wob[] = 
{
    0.5942778786400943, 0.6215907685815624, 0.6377766702163756, 0.6458754776578299, 0.6593837156149394, 0.6913153957016749, 0.6928195128806074, 0.6889809314165064, 0.6986588422997559, 0.7208006992465622, 
    0.737788494863834, 0.7464064452788496, 0.7770910546806311, 0.7712073750858836, 0.7758558619638954, 0.8086186707320293, 0.8310338565635296, 0.8387438916584461, 0.8703928926783518, 0.9017936553308619, 
    0.8937433463954164, 0.9280687991374716, 0.9247986935092276, 0.9421546129166595, 1.0084380769112697, 1.015329956685102, 1.0212580247403102, 1.0548084085410445, 1.0680912390833106, 1.107351182295996, 
    1.107158137467735, 1.1531708219072334, 1.1697836768424144, 1.2110348205412274, 1.2405829542321916, 1.2718508345725337, 1.286216184369616, 1.33070519152514, 1.3564410083791387, 1.3468678617486354
};

float NpartNormalizer::weight(int npp, TString compare_case)
{
    if (npp < lowest_npp || npp >= lowest_npp + 40) return 0;
    if (compare_case == "ox")   return ratio_ox[npp-lowest_npp];
    if (compare_case == "oxb")  return ratio_oxb[npp-lowest_npp];
    if (compare_case == "obx")  return ratio_obx[npp-lowest_npp];
    if (compare_case == "obxb") return ratio_obxb[npp-lowest_npp];

    if (compare_case == "x")  return ratio_x[npp-lowest_npp];
    if (compare_case == "xb") return ratio_xb[npp-lowest_npp];
    if (compare_case == "o")  return ratio_o[npp-lowest_npp];
    if (compare_case == "ob") return ratio_ob[npp-lowest_npp];

    if (compare_case == "woallx_for_wx")  return ratio_woallx_for_wx[npp-lowest_npp];
    if (compare_case == "woallx_for_wxb") return ratio_woallx_for_wxb[npp-lowest_npp];
    if (compare_case == "woallo_for_wo")  return ratio_woallo_for_wo[npp-lowest_npp];
    if (compare_case == "woallo_for_wob") return ratio_woallo_for_wob[npp-lowest_npp];

    return 0;
}


