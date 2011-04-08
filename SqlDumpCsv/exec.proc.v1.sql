exec stat_hot_gprs '[PS总流量] desc','100','PS总流量100'
exec stat_hot_gprs '[平均分配PDCH] desc','100','平均分配PDCH100'
exec stat_hot_gprs '[PS立即指派成功率] asc','10000'
exec stat_hot_gprs '[上行TBF建立成功率] asc','10000'
exec stat_hot_gprs '[上行TBF建立成功率] asc','10000'
exec stat_hot_gprs '[上行TBF掉线率] asc','10000'
exec stat_hot_gprs '[EDGE下行速率] asc','10000'



指标	指标定义	目标
上行TBF接入成功率	1-(PREJTFI+PREJOTH)/PSCHREQ×100%	≥96%
下行TBF接入成功率	1-FAILDLTBFEST/DLTBFEST×100%	≥96%
PS立即指派成功率	1-REJPSIMMASS/(PSIMMASS+REJPSIMMASS)×100%	≥96%
上行TBF掉线率	IAULREL/(PSCHREQ-PREJTFI-PREJOTH)×100%	≤1.5%
EDGE下行速率	RLC层平均吞吐率	≥80kbit/s
投诉黑点解决率	黑点问题解决数/黑点问题总数×100%	≥90%
数据业务黑点的优化规范
