
select * into gprsredian_ps
from (
SELECT b.*,a.PS����ָ���� as PS����ָ����,a.PS����ָ��ʧ���� as   PS����ָ��ʧ����
FROM dbo.PS as a,dbo.gprsredian as b
where a.ʱ��=b.ʱ�� and a.С����=b.С����
) as c

exec ExtractTable 'gprsredian_ps','[EDGE��������] asc','1','EDGE��������1'