#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>

#define	MAXODR	1000	/* 並列の場合、1000の桁までの値を対象とする。*/

double	e24_tbl[] = {
	1.0, 1.1, 1.2, 1.3, 1.5, 1.6, 1.8, 2.0, 2.2, 2.4, 2.7, 3.0,
	3.3, 3.6, 3.9, 4.3, 4.7, 5.1, 5.6, 6.2, 6.8, 7.5, 8.2, 9.1
	};

double	para(double r1, double r2)	/* 並列の抵抗計算 */
{
	return r1*r2/(r1+r2);
}

double	get_pr(double r1,double r2)	/* 並列の抵抗計算 */
{
	return r1*r2/(r2-r1);
}

double	aprx(double r)	/* rに最も近い値をＥ２４系列の中から見つける */
{
	double	n_r,t_r,order;
	char	n;

	if(r > MAXODR)	/* 桁数のチェック */
		return 0;
	for(order = 0.01; order <= MAXODR; order = order*10)
		if((n_r = r/order) < 10)	/* 係数を求める */
			break;
	if(n_r >= 9.5)	/* 係数が９．５以上ならば一つうえの桁の１．０に */
		return order*10.0;
	for(n = 0; n < 24; n++){	/* Ｅ２４系列にある値と比較する */
		t_r = e24_tbl[n];
		if(t_r >= n_r){	/* 誤差を調べる */
			if(n == 0)	/* just 1.0 */
				break;
			if((n_r - e24_tbl[n-1]) < (t_r - n_r))
				t_r = e24_tbl[n-1];
			break;
		}
	}
	return t_r*order;
}

void	put_r1(double r, double err)	/* 単独の抵抗の場合の表示 */
{
	if(fabs(err) < 0.000001)
		printf("%6.1f alone\n",r);
	else
		printf("%6.1f alone          (%+4.3f%%)\n",r,100*err);
}

void	put_pr2(double r1, double r2, double err)	/* 並列の場合の表示 */
{
	if(fabs(err) < 0.000001)
		printf("%6.1f   //   %6.1f\n",r1,r2);
	else
		printf("%6.1f   //   %6.1f (%+4.3f%%)\n",r1,r2,100*err);
}

void	put_sr2(double r1, double r2, double err)	/* 直列の場合の表示 */
{
	if(r1 < 0.099)
		printf("%6.3f     +   ",r1);
	else if(r1 < 0.99)
		printf("%6.2f    +   ",r1);
	else
		printf("%6.1f    +   ",r1);
	if(r2 < 0.099)
		printf("%6.3f",r2);
	else if(r2 < 0.99)
		printf("%6.2f",r2);
	else
		printf("%6.1f",r2);
	if(fabs(err) >= 0.000001)
		printf("  (%+4.3f%%)",100*err);
	putchar('\n');
}

void	r_find(double r, double acc)	/* ペアとなる値を見つける */
{
	double	ex_r, t_r, ap_r, err, order;
	char	n;

	for(order = 0.01; order <= MAXODR; order = order*10){
		for(n = 0; n < 24; n++){	/* Ｅ２４系列の小さい方から探す */
			t_r = e24_tbl[n]*order;
			err = t_r - r;
			if(fabs(err) <= acc)	/* 単独で精度が得られた場合 */
				put_r1(t_r, err/r);
			else if(err < 0){	/* 値が不足な場合は直列の組合わせを探す */
				ex_r = r - t_r;
				if((ap_r = aprx(ex_r)) != 0){
					if(ap_r < t_r - 0.001) /* 組合わせの重複はスキップ */
						continue;
					err = t_r + ap_r - r;	/* 合成値の誤差をチェック */
					if(fabs(err) <= acc)	/* 指定精度以内なら表示 */
						put_sr2(ap_r, t_r, err/r);
				}
			}
			else{	/* 値が大きい場合は並列の組合わせを探す */
				ex_r = get_pr(r,t_r);
				if((ap_r = aprx(ex_r)) != 0){
					if(ap_r > t_r + 0.001)	/* 組合わせの重複はスキップ */
						continue;
					err = para(t_r, ap_r) - r;	/* 合成値の誤差をチェック */
					if(fabs(err) <= acc)		/* 指定精度以内なら表示 */
						put_pr2(ap_r, t_r, err/r);
				}
			}
		}
	}
}

void	main(int argc, char *argv[])
{
	double	reg, rate;

	switch(argc) {
	case 2:
		rate = 5.0;
		reg = atof(argv[1]);
		break;
	case 3:
		rate = atof(argv[2]);
		reg = atof(argv[1]);
		break;
	default:
		puts("usage: rr val accuracy");
		exit(1);
	}

	if(reg <= 1 || reg > 10000){	/* 抵抗値の範囲をチェック */
		puts("registance value is over range.");
		exit(254);
	}
	if(rate <= 0){	/* 精度の範囲をチェック */
		puts("illegal accuracy value.");
		exit(253);
	}
	r_find(reg,reg*rate/100.0);	/* 抵抗値の検索 */
}
