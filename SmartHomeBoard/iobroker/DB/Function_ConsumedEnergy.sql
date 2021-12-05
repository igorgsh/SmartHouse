-- FUNCTION: public.consumedenergy(date, date)

-- DROP FUNCTION IF EXISTS public.consumedenergy(date, date);

CREATE OR REPLACE FUNCTION public.consumedenergy(
	"DateFrom" date,
	"DateTo" date)
    RETURNS TABLE(source text, indicator text, isday boolean, dt date, consumption double precision) 
    LANGUAGE 'plpgsql'
    COST 100
    VOLATILE PARALLEL UNSAFE
    ROWS 1000

AS $BODY$
begin
	return query (
select t1.source as source, t1.indicator as indicator, t1.isday as isday, t1.dt as dt, SUM(t1.cons) as consumption
from(
select t.source, t.indicator, t.isday, t.dt, (t.end-t.beg) as cons
from (
select e.source, e.indicator, e.isday, e.dt, min(e.val) as beg, max (e.val) as end
from "EnergyView" e
where (e.dt >= "DateFrom" AND e.dt<= "DateTo") and e.indicator='Energy'
and e.hour != 23
group by e.source, e.indicator, e.isday, e.dt
union all
select e.source, e.indicator, e.isday, e.dt, min(e.val) as beg, max(e.val) as end
from "EnergyView" e
where (e.dt >= "DateFrom" AND e.dt<= "DateTo") and e.indicator='Energy'
and e.hour = 23
group by e.source, e.indicator, e.isday, e.dt
) t
) t1
group by t1.source, t1.indicator, t1.isday, t1.dt
      );
end;
$BODY$;

ALTER FUNCTION public.consumedenergy(date, date)
    OWNER TO iobroker;
