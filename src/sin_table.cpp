#define MY_PI 3.14159265359
#define MY_HALF_PI 1.5707963268

// table of sines in 0.01 radian increments
extern const double sin_table[629] = { 
	0.0000000e+000, 
	9.9998333e-003, 
	1.9998667e-002, 
	2.9995500e-002, 
	3.9989334e-002, 
	4.9979169e-002, 
	5.9964006e-002, 
	6.9942847e-002, 
	7.9914694e-002, 
	8.9878549e-002, 
	9.9833417e-002, 
	1.0977830e-001, 
	1.1971221e-001, 
	1.2963414e-001, 
	1.3954311e-001, 
	1.4943813e-001, 
	1.5931821e-001, 
	1.6918235e-001, 
	1.7902957e-001, 
	1.8885889e-001, 
	1.9866933e-001, 
	2.0845990e-001, 
	2.1822962e-001, 
	2.2797752e-001, 
	2.3770263e-001, 
	2.4740396e-001, 
	2.5708055e-001, 
	2.6673144e-001, 
	2.7635565e-001, 
	2.8595223e-001, 
	2.9552021e-001, 
	3.0505864e-001, 
	3.1456656e-001, 
	3.2404303e-001, 
	3.3348709e-001, 
	3.4289781e-001, 
	3.5227423e-001, 
	3.6161543e-001, 
	3.7092047e-001, 
	3.8018842e-001, 
	3.8941834e-001, 
	3.9860933e-001, 
	4.0776045e-001, 
	4.1687080e-001, 
	4.2593947e-001, 
	4.3496553e-001, 
	4.4394811e-001, 
	4.5288629e-001, 
	4.6177918e-001, 
	4.7062589e-001, 
	4.7942554e-001, 
	4.8817725e-001, 
	4.9688014e-001, 
	5.0553334e-001, 
	5.1413599e-001, 
	5.2268723e-001, 
	5.3118620e-001, 
	5.3963205e-001, 
	5.4802394e-001, 
	5.5636102e-001, 
	5.6464247e-001, 
	5.7286746e-001, 
	5.8103516e-001, 
	5.8914476e-001, 
	5.9719544e-001, 
	6.0518641e-001, 
	6.1311685e-001, 
	6.2098599e-001, 
	6.2879302e-001, 
	6.3653718e-001, 
	6.4421769e-001, 
	6.5183377e-001, 
	6.5938467e-001, 
	6.6686964e-001, 
	6.7428791e-001, 
	6.8163876e-001, 
	6.8892145e-001, 
	6.9613524e-001, 
	7.0327942e-001, 
	7.1035327e-001, 
	7.1735609e-001, 
	7.2428717e-001, 
	7.3114583e-001, 
	7.3793137e-001, 
	7.4464312e-001, 
	7.5128041e-001, 
	7.5784256e-001, 
	7.6432894e-001, 
	7.7073888e-001, 
	7.7707175e-001, 
	7.8332691e-001, 
	7.8950374e-001, 
	7.9560162e-001, 
	8.0161994e-001, 
	8.0755810e-001, 
	8.1341550e-001, 
	8.1919157e-001, 
	8.2488571e-001, 
	8.3049737e-001, 
	8.3602598e-001, 
	8.4147098e-001, 
	8.4683184e-001, 
	8.5210802e-001, 
	8.5729899e-001, 
	8.6240423e-001, 
	8.6742323e-001, 
	8.7235548e-001, 
	8.7720050e-001, 
	8.8195781e-001, 
	8.8662691e-001, 
	8.9120736e-001, 
	8.9569869e-001, 
	9.0010044e-001, 
	9.0441219e-001, 
	9.0863350e-001, 
	9.1276394e-001, 
	9.1680311e-001, 
	9.2075060e-001, 
	9.2460601e-001, 
	9.2836897e-001, 
	9.3203909e-001, 
	9.3561600e-001, 
	9.3909936e-001, 
	9.4248880e-001, 
	9.4578400e-001, 
	9.4898462e-001, 
	9.5209034e-001, 
	9.5510086e-001, 
	9.5801586e-001, 
	9.6083506e-001, 
	9.6355819e-001, 
	9.6618495e-001, 
	9.6871510e-001, 
	9.7114838e-001, 
	9.7348454e-001, 
	9.7572336e-001, 
	9.7786460e-001, 
	9.7990806e-001, 
	9.8185353e-001, 
	9.8370081e-001, 
	9.8544973e-001, 
	9.8710010e-001, 
	9.8865176e-001, 
	9.9010456e-001, 
	9.9145835e-001, 
	9.9271299e-001, 
	9.9386836e-001, 
	9.9492435e-001, 
	9.9588084e-001, 
	9.9673775e-001, 
	9.9749499e-001, 
	9.9815247e-001, 
	9.9871014e-001, 
	9.9916795e-001, 
	9.9952583e-001, 
	9.9978376e-001, 
	9.9994172e-001, 
	9.9999968e-001, 
	9.9995765e-001, 
	9.9981562e-001, 
	9.9957360e-001, 
	9.9923163e-001, 
	9.9878974e-001, 
	9.9824797e-001, 
	9.9760638e-001, 
	9.9686503e-001, 
	9.9602399e-001, 
	9.9508335e-001, 
	9.9404320e-001, 
	9.9290365e-001, 
	9.9166481e-001, 
	9.9032680e-001, 
	9.8888977e-001, 
	9.8735384e-001, 
	9.8571918e-001, 
	9.8398595e-001, 
	9.8215432e-001, 
	9.8022447e-001, 
	9.7819661e-001, 
	9.7607092e-001, 
	9.7384763e-001, 
	9.7152696e-001, 
	9.6910913e-001, 
	9.6659439e-001, 
	9.6398300e-001, 
	9.6127520e-001, 
	9.5847128e-001, 
	9.5557152e-001, 
	9.5257619e-001, 
	9.4948561e-001, 
	9.4630009e-001, 
	9.4301993e-001, 
	9.3964547e-001, 
	9.3617705e-001, 
	9.3261501e-001, 
	9.2895972e-001, 
	9.2521152e-001, 
	9.2137081e-001, 
	9.1743796e-001, 
	9.1341336e-001, 
	9.0929743e-001, 
	9.0509056e-001, 
	9.0079319e-001, 
	8.9640574e-001, 
	8.9192865e-001, 
	8.8736237e-001, 
	8.8270735e-001, 
	8.7796406e-001, 
	8.7313298e-001, 
	8.6821458e-001, 
	8.6320937e-001, 
	8.5811783e-001, 
	8.5294048e-001, 
	8.4767784e-001, 
	8.4233043e-001, 
	8.3689879e-001, 
	8.3138346e-001, 
	8.2578499e-001, 
	8.2010395e-001, 
	8.1434089e-001, 
	8.0849640e-001, 
	8.0257107e-001, 
	7.9656547e-001, 
	7.9048022e-001, 
	7.8431593e-001, 
	7.7807320e-001, 
	7.7175266e-001, 
	7.6535495e-001, 
	7.5888071e-001, 
	7.5233058e-001, 
	7.4570521e-001, 
	7.3900528e-001, 
	7.3223144e-001, 
	7.2538439e-001, 
	7.1846479e-001, 
	7.1147335e-001, 
	7.0441077e-001, 
	6.9727774e-001, 
	6.9007498e-001, 
	6.8280322e-001, 
	6.7546318e-001, 
	6.6805559e-001, 
	6.6058120e-001, 
	6.5304075e-001, 
	6.4543500e-001, 
	6.3776470e-001, 
	6.3003063e-001, 
	6.2223356e-001, 
	6.1437426e-001, 
	6.0645352e-001, 
	5.9847214e-001, 
	5.9043092e-001, 
	5.8233065e-001, 
	5.7417215e-001, 
	5.6595623e-001, 
	5.5768372e-001, 
	5.4935544e-001, 
	5.4097222e-001, 
	5.3253491e-001, 
	5.2404434e-001, 
	5.1550137e-001, 
	5.0690685e-001, 
	4.9826164e-001, 
	4.8956661e-001, 
	4.8082261e-001, 
	4.7203054e-001, 
	4.6319126e-001, 
	4.5430567e-001, 
	4.4537464e-001, 
	4.3639908e-001, 
	4.2737988e-001, 
	4.1831794e-001, 
	4.0921417e-001, 
	4.0006948e-001, 
	3.9088478e-001, 
	3.8166099e-001, 
	3.7239904e-001, 
	3.6309985e-001, 
	3.5376435e-001, 
	3.4439347e-001, 
	3.3498815e-001, 
	3.2554933e-001, 
	3.1607796e-001, 
	3.0657499e-001, 
	2.9704135e-001, 
	2.8747801e-001, 
	2.7788593e-001, 
	2.6826605e-001, 
	2.5861935e-001, 
	2.4894679e-001, 
	2.3924933e-001, 
	2.2952795e-001, 
	2.1978361e-001, 
	2.1001730e-001, 
	2.0022998e-001, 
	1.9042265e-001, 
	1.8059627e-001, 
	1.7075183e-001, 
	1.6089031e-001, 
	1.5101271e-001, 
	1.4112001e-001, 
	1.3121319e-001, 
	1.2129326e-001, 
	1.1136119e-001, 
	1.0141799e-001, 
	9.1464642e-002, 
	8.1502152e-002, 
	7.1531511e-002, 
	6.1553717e-002, 
	5.1569768e-002, 
	4.1580662e-002, 
	3.1587398e-002, 
	2.1590976e-002, 
	1.1592394e-002, 
	1.5926529e-003, 
	-8.4072474e-003, 
	-1.8406307e-002, 
	-2.8403526e-002, 
	-3.8397905e-002, 
	-4.8388443e-002, 
	-5.8374143e-002, 
	-6.8354006e-002, 
	-7.8327033e-002, 
	-8.8292228e-002, 
	-9.8248594e-002, 
	-1.0819513e-001, 
	-1.1813086e-001, 
	-1.2805476e-001, 
	-1.3796587e-001, 
	-1.4786317e-001, 
	-1.5774569e-001, 
	-1.6761244e-001, 
	-1.7746242e-001, 
	-1.8729466e-001, 
	-1.9710817e-001, 
	-2.0690197e-001, 
	-2.1667508e-001, 
	-2.2642652e-001, 
	-2.3615532e-001, 
	-2.4586050e-001, 
	-2.5554110e-001, 
	-2.6519615e-001, 
	-2.7482467e-001, 
	-2.8442571e-001, 
	-2.9399831e-001, 
	-3.0354151e-001, 
	-3.1305436e-001, 
	-3.2253590e-001, 
	-3.3198519e-001, 
	-3.4140128e-001, 
	-3.5078323e-001, 
	-3.6013010e-001, 
	-3.6944096e-001, 
	-3.7871487e-001, 
	-3.8795092e-001, 
	-3.9714817e-001, 
	-4.0630570e-001, 
	-4.1542261e-001, 
	-4.2449797e-001, 
	-4.3353088e-001, 
	-4.4252044e-001, 
	-4.5146575e-001, 
	-4.6036591e-001, 
	-4.6922004e-001, 
	-4.7802725e-001, 
	-4.8678665e-001, 
	-4.9549737e-001, 
	-5.0415855e-001, 
	-5.1276931e-001, 
	-5.2132879e-001, 
	-5.2983614e-001, 
	-5.3829051e-001, 
	-5.4669105e-001, 
	-5.5503692e-001, 
	-5.6332728e-001, 
	-5.7156132e-001, 
	-5.7973820e-001, 
	-5.8785710e-001, 
	-5.9591722e-001, 
	-6.0391775e-001, 
	-6.1185789e-001, 
	-6.1973684e-001, 
	-6.2755382e-001, 
	-6.3530805e-001, 
	-6.4299874e-001, 
	-6.5062514e-001, 
	-6.5818647e-001, 
	-6.6568199e-001, 
	-6.7311093e-001, 
	-6.8047257e-001, 
	-6.8776616e-001, 
	-6.9499097e-001, 
	-7.0214629e-001, 
	-7.0923139e-001, 
	-7.1624557e-001, 
	-7.2318812e-001, 
	-7.3005836e-001, 
	-7.3685559e-001, 
	-7.4357914e-001, 
	-7.5022833e-001, 
	-7.5680250e-001, 
	-7.6330098e-001, 
	-7.6972314e-001, 
	-7.7606833e-001, 
	-7.8233591e-001, 
	-7.8852525e-001, 
	-7.9463575e-001, 
	-8.0066678e-001, 
	-8.0661775e-001, 
	-8.1248805e-001, 
	-8.1827711e-001, 
	-8.2398434e-001, 
	-8.2960917e-001, 
	-8.3515105e-001, 
	-8.4060940e-001, 
	-8.4598370e-001, 
	-8.5127340e-001, 
	-8.5647797e-001, 
	-8.6159690e-001, 
	-8.6662967e-001, 
	-8.7157577e-001, 
	-8.7643472e-001, 
	-8.8120603e-001, 
	-8.8588921e-001, 
	-8.9048381e-001, 
	-8.9498936e-001, 
	-8.9940541e-001, 
	-9.0373152e-001, 
	-9.0796726e-001, 
	-9.1211220e-001, 
	-9.1616594e-001, 
	-9.2012805e-001, 
	-9.2399816e-001, 
	-9.2777586e-001, 
	-9.3146079e-001, 
	-9.3505258e-001, 
	-9.3855086e-001, 
	-9.4195528e-001, 
	-9.4526551e-001, 
	-9.4848122e-001, 
	-9.5160207e-001, 
	-9.5462777e-001, 
	-9.5755801e-001, 
	-9.6039249e-001, 
	-9.6313093e-001, 
	-9.6577306e-001, 
	-9.6831861e-001, 
	-9.7076734e-001, 
	-9.7311898e-001, 
	-9.7537332e-001, 
	-9.7753012e-001, 
	-9.7958916e-001, 
	-9.8155025e-001, 
	-9.8341319e-001, 
	-9.8517778e-001, 
	-9.8684386e-001, 
	-9.8841125e-001, 
	-9.8987981e-001, 
	-9.9124937e-001, 
	-9.9251981e-001, 
	-9.9369100e-001, 
	-9.9476283e-001, 
	-9.9573517e-001, 
	-9.9660795e-001, 
	-9.9738106e-001, 
	-9.9805444e-001, 
	-9.9862801e-001, 
	-9.9910172e-001, 
	-9.9947552e-001, 
	-9.9974938e-001, 
	-9.9992326e-001, 
	-9.9999715e-001, 
	-9.9997104e-001, 
	-9.9984493e-001, 
	-9.9961884e-001, 
	-9.9929279e-001, 
	-9.9886681e-001, 
	-9.9834094e-001, 
	-9.9771525e-001, 
	-9.9698978e-001, 
	-9.9616461e-001, 
	-9.9523983e-001, 
	-9.9421552e-001, 
	-9.9309179e-001, 
	-9.9186876e-001, 
	-9.9054654e-001, 
	-9.8912526e-001, 
	-9.8760507e-001, 
	-9.8598613e-001, 
	-9.8426858e-001, 
	-9.8245261e-001, 
	-9.8053840e-001, 
	-9.7852613e-001, 
	-9.7641601e-001, 
	-9.7420825e-001, 
	-9.7190307e-001, 
	-9.6950070e-001, 
	-9.6700138e-001, 
	-9.6440536e-001, 
	-9.6171290e-001, 
	-9.5892427e-001, 
	-9.5603975e-001, 
	-9.5305963e-001, 
	-9.4998420e-001, 
	-9.4681378e-001, 
	-9.4354867e-001, 
	-9.4018921e-001, 
	-9.3673573e-001, 
	-9.3318858e-001, 
	-9.2954811e-001, 
	-9.2581468e-001, 
	-9.2198868e-001, 
	-9.1807047e-001, 
	-9.1406047e-001, 
	-9.0995905e-001, 
	-9.0576664e-001, 
	-9.0148366e-001, 
	-8.9711052e-001, 
	-8.9264768e-001, 
	-8.8809557e-001, 
	-8.8345466e-001, 
	-8.7872539e-001, 
	-8.7390826e-001, 
	-8.6900374e-001, 
	-8.6401232e-001, 
	-8.5893449e-001, 
	-8.5377078e-001, 
	-8.4852169e-001, 
	-8.4318774e-001, 
	-8.3776948e-001, 
	-8.3226744e-001, 
	-8.2668218e-001, 
	-8.2101425e-001, 
	-8.1526421e-001, 
	-8.0943266e-001, 
	-8.0352016e-001, 
	-7.9752730e-001, 
	-7.9145470e-001, 
	-7.8530295e-001, 
	-7.7907267e-001, 
	-7.7276449e-001, 
	-7.6637903e-001, 
	-7.5991693e-001, 
	-7.5337884e-001, 
	-7.4676541e-001, 
	-7.4007731e-001, 
	-7.3331520e-001, 
	-7.2647976e-001, 
	-7.1957167e-001, 
	-7.1259163e-001, 
	-7.0554033e-001, 
	-6.9841847e-001, 
	-6.9122677e-001, 
	-6.8396595e-001, 
	-6.7663674e-001, 
	-6.6923986e-001, 
	-6.6177605e-001, 
	-6.5424608e-001, 
	-6.4665067e-001, 
	-6.3899060e-001, 
	-6.3126664e-001, 
	-6.2347955e-001, 
	-6.1563011e-001, 
	-6.0771910e-001, 
	-5.9974733e-001, 
	-5.9171558e-001, 
	-5.8362466e-001, 
	-5.7547538e-001, 
	-5.6726855e-001, 
	-5.5900500e-001, 
	-5.5068554e-001, 
	-5.4231102e-001, 
	-5.3388227e-001, 
	-5.2540013e-001, 
	-5.1686544e-001, 
	-5.0827908e-001, 
	-4.9964188e-001, 
	-4.9095472e-001, 
	-4.8221847e-001, 
	-4.7343400e-001, 
	-4.6460218e-001, 
	-4.5572390e-001, 
	-4.4680005e-001, 
	-4.3783152e-001, 
	-4.2881921e-001, 
	-4.1976402e-001, 
	-4.1066685e-001, 
	-4.0152861e-001, 
	-3.9235022e-001, 
	-3.8313260e-001, 
	-3.7387666e-001, 
	-3.6458334e-001, 
	-3.5525356e-001, 
	-3.4588825e-001, 
	-3.3648836e-001, 
	-3.2705481e-001, 
	-3.1758857e-001, 
	-3.0809056e-001, 
	-2.9856174e-001, 
	-2.8900307e-001, 
	-2.7941550e-001, 
	-2.6979998e-001, 
	-2.6015749e-001, 
	-2.5048898e-001, 
	-2.4079543e-001, 
	-2.3107779e-001, 
	-2.2133704e-001, 
	-2.1157417e-001, 
	-2.0179013e-001, 
	-1.9198592e-001, 
	-1.8216250e-001, 
	-1.7232088e-001, 
	-1.6246202e-001, 
	-1.5258691e-001, 
	-1.4269654e-001, 
	-1.3279191e-001, 
	-1.2287400e-001, 
	-1.1294379e-001, 
	-1.0300230e-001, 
	-9.3050503e-002, 
	-8.3089403e-002, 
	-7.3119994e-002, 
	-6.3143272e-002, 
	-5.3160237e-002, 
	-4.3171885e-002, 
	-3.3179217e-002, 
	-2.3183230e-002, 
	-1.3184925e-002, 
	-3.1853018e-003
	};

double fsin(double rad)
{
	int index;

	while (rad > 2 * MY_PI)
		rad -= 2 * MY_PI;

	while (rad < 0)
		rad += 2 * MY_PI;

	index = (int)(rad * 100);

	return sin_table[index];
}

double fcos(double rad)
{
	int index;

	rad = MY_HALF_PI - rad;

	while (rad > 2 * MY_PI)
		rad -= 2 * MY_PI;

	while (rad < 0)
		rad += 2 * MY_PI;

	index = (int)(rad * 100);

	return sin_table[index];
}

float InvSqrt(float x)
{
    float xhalf = 0.5f * x;
	int i = *(int*)&x;
    i = 0x5f3759df - (i >> 1);
    x = *(float *)&i;
    x = x * (1.5f - xhalf * x * x);
    return x;
}

float abs32(float val)
{
	int ival = (*(int*)&val & 0x7FFFFFFF);
	return *(float *)&ival;
}

int abs32(int val)
{
	int mask = (val >> 31);
	return (val + mask) ^ mask;
}