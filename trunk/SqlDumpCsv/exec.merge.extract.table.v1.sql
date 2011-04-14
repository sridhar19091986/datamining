
select * into gprsredian_ps
from (
SELECT b.*,a.PS立即指派数 as PS立即指派数,a.PS立即指派失败数 as   PS立即指派失败数
FROM dbo.PS as a,dbo.gprsredian as b
where a.时间=b.时间 and a.小区号=b.小区号
) as c

exec ExtractTable 'gprsredian_ps','[EDGE下行速率] asc','1','EDGE下行速率1'