use tempdb
exec stat_hot_gprs '[PS������] desc','10000','PS������10000'
exec stat_hot_gprs '[ƽ������PDCH] desc','10000','ƽ������PDCH10000'
exec stat_hot_gprs '[PS����ָ�ɳɹ���] asc','10000','PS����ָ�ɳɹ���10000'
exec stat_hot_gprs '[����TBF�����ɹ���] asc','10000','����TBF�����ɹ���10000'
exec stat_hot_gprs '[����TBF�����ɹ���] asc','10000','����TBF�����ɹ���10000'
exec stat_hot_gprs '[����TBF������] asc','10000','����TBF������10000'
exec stat_hot_gprs '[EDGE��������] asc','10000','EDGE��������10000'



ָ��	ָ�궨��	Ŀ��
����TBF����ɹ���	1-(PREJTFI+PREJOTH)/PSCHREQ��100%	��96%
����TBF����ɹ���	1-FAILDLTBFEST/DLTBFEST��100%	��96%
PS����ָ�ɳɹ���	1-REJPSIMMASS/(PSIMMASS+REJPSIMMASS)��100%	��96%
����TBF������	IAULREL/(PSCHREQ-PREJTFI-PREJOTH)��100%	��1.5%
EDGE��������	RLC��ƽ��������	��80kbit/s
Ͷ�ߺڵ�����	�ڵ���������/�ڵ�����������100%	��90%
����ҵ��ڵ���Ż��淶