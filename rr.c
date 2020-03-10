#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>

#define	MAXODR	1000	/* ‰∏¶Âàó„ÅÆÂ†¥Âêà„ÄÅ1000„ÅÆÊ°Å„Åæ„Åß„ÅÆÂÄ§„ÇíÂØæË±°„Å®„Åô„Çã„ÄÇ*/

double	e24_tbl[] = {
	1.0, 1.1, 1.2, 1.3, 1.5, 1.6, 1.8, 2.0, 2.2, 2.4, 2.7, 3.0,
	3.3, 3.6, 3.9, 4.3, 4.7, 5.1, 5.6, 6.2, 6.8, 7.5, 8.2, 9.1
	};

double	para(double r1, double r2)	/* ‰∏¶Âàó„ÅÆÊäµÊäóË®àÁÆóÔºàıÄëºÔºâ */
{
	return r1*r2/(r1+r2);
}

double	get_pr(double r1,double r2)	/* ‰∏¶Âàó„ÅÆÊäµÊäóË®àÁÆóÔºàıÄëΩÔºâ */
{
	return r1*r2/(r2-r1);
}

double	aprx(double r)	/* r„Å´ÊúÄ„ÇÇËøë„ÅÑÂÄ§„ÇíÔº•ÔºíÔºîÁ≥ªÂàó„ÅÆ‰∏≠„Åã„ÇâË¶ã„Å§„Åë„Çã */
{
	double	n_r,t_r,order;
	char	n;

	if(r > MAXODR)	/* Ê°ÅÊï∞„ÅÆ„ÉÅ„Çß„ÉÉ„ÇØ */
		return 0;
	for(order = 0.01; order <= MAXODR; order = order*10)
		if((n_r = r/order) < 10)	/* ‰øÇÊï∞„ÇíÊ±Ç„ÇÅ„Çã */
			break;
	if(n_r >= 9.5)	/* ‰øÇÊï∞„ÅåÔºôÔºéÔºï‰ª•‰∏ä„Å™„Çâ„Å∞‰∏Ä„Å§„ÅÜ„Åà„ÅÆÊ°Å„ÅÆÔºëÔºéÔºê„Å´ */
		return order*10.0;
	for(n = 0; n < 24; n++){	/* Ôº•ÔºíÔºîÁ≥ªÂàó„Å´„ÅÇ„ÇãÂÄ§„Å®ÊØîËºÉ„Åô„Çã */
		t_r = e24_tbl[n];
		if(t_r >= n_r){	/* Ë™§Â∑Æ„ÇíË™ø„Åπ„Çã */
			if(n == 0)	/* just 1.0 */
				break;
			if((n_r - e24_tbl[n-1]) < (t_r - n_r))
				t_r = e24_tbl[n-1];
			break;
		}
	}
	return t_r*order;
}

void	put_r1(double r, double err)	/* ÂçòÁã¨„ÅÆÊäµÊäó„ÅÆÂ†¥Âêà„ÅÆË°®Á§∫ */
{
	if(fabs(err) < 0.000001)
		printf("%6.1f alone\n",r);
	else
		printf("%6.1f alone          (%+4.3f%%)\n",r,100*err);
}

void	put_pr2(double r1, double r2, double err)	/* ‰∏¶Âàó„ÅÆÂ†¥Âêà„ÅÆË°®Á§∫ */
{
	if(fabs(err) < 0.000001)
		printf("%6.1f   //   %6.1f\n",r1,r2);
	else
		printf("%6.1f   //   %6.1f (%+4.3f%%)\n",r1,r2,100*err);
}

void	put_sr2(double r1, double r2, double err)	/* Áõ¥Âàó„ÅÆÂ†¥Âêà„ÅÆË°®Á§∫ */
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

void	r_find(double r, double acc)	/* „Éö„Ç¢„Å®„Å™„ÇãÂÄ§„ÇíË¶ã„Å§„Åë„Çã */
{
	double	ex_r, t_r, ap_r, err, order;
	char	n;

	for(order = 0.01; order <= MAXODR; order = order*10){
		for(n = 0; n < 24; n++){	/* Ôº•ÔºíÔºîÁ≥ªÂàó„ÅÆÂ∞è„Åï„ÅÑÊñπ„Åã„ÇâÊé¢„Åô */
			t_r = e24_tbl[n]*order;
			err = t_r - r;
			if(fabs(err) <= acc)	/* ÂçòÁã¨„ÅßÁ≤æÂ∫¶„ÅåÂæó„Çâ„Çå„ÅüÂ†¥Âêà */
				put_r1(t_r, err/r);
			else if(err < 0){	/* ÂÄ§„Åå‰∏çË∂≥„Å™Â†¥Âêà„ÅØÁõ¥Âàó„ÅÆÁµÑÂêà„Çè„Åõ„ÇíÊé¢„Åô */
				ex_r = r - t_r;
				if((ap_r = aprx(ex_r)) != 0){
					if(ap_r < t_r - 0.001) /* ÁµÑÂêà„Çè„Åõ„ÅÆÈáçË§á„ÅØ„Çπ„Ç≠„ÉÉ„Éó */
						continue;
					err = t_r + ap_r - r;	/* ÂêàÊàêÂÄ§„ÅÆË™§Â∑Æ„Çí„ÉÅ„Çß„ÉÉ„ÇØ */
					if(fabs(err) <= acc)	/* ÊåáÂÆöÁ≤æÂ∫¶‰ª•ÂÜÖ„Å™„ÇâË°®Á§∫ */
						put_sr2(ap_r, t_r, err/r);
				}
			}
			else{	/* ÂÄ§„ÅåÂ§ß„Åç„ÅÑÂ†¥Âêà„ÅØ‰∏¶Âàó„ÅÆÁµÑÂêà„Çè„Åõ„ÇíÊé¢„Åô */
				ex_r = get_pr(r,t_r);
				if((ap_r = aprx(ex_r)) != 0){
					if(ap_r > t_r + 0.001)	/* ÁµÑÂêà„Çè„Åõ„ÅÆÈáçË§á„ÅØ„Çπ„Ç≠„ÉÉ„Éó */
						continue;
					err = para(t_r, ap_r) - r;	/* ÂêàÊàêÂÄ§„ÅÆË™§Â∑Æ„Çí„ÉÅ„Çß„ÉÉ„ÇØ */
					if(fabs(err) <= acc)		/* ÊåáÂÆöÁ≤æÂ∫¶‰ª•ÂÜÖ„Å™„ÇâË°®Á§∫ */
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

	if(reg <= 1 || reg > 10000){	/* ÊäµÊäóÂÄ§„ÅÆÁØÑÂõ≤„Çí„ÉÅ„Çß„ÉÉ„ÇØ */
		puts("registance value is over range.");
		exit(254);
	}
	if(rate <= 0){	/* Á≤æÂ∫¶„ÅÆÁØÑÂõ≤„Çí„ÉÅ„Çß„ÉÉ„ÇØ */
		puts("illegal accuracy value.");
		exit(253);
	}
	r_find(reg,reg*rate/100.0);	/* ÊäµÊäóÂÄ§„ÅÆÊ§úÁ¥¢ */
}
