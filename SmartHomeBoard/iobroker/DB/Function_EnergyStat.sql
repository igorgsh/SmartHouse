-- FUNCTION: public.energystat(date, date)

-- DROP FUNCTION IF EXISTS public.energystat(date, date);

CREATE OR REPLACE FUNCTION public.energystat(
	"DateFrom" date,
	"DateTo" date)
    RETURNS TABLE(source text, dt date, isday boolean, consumption double precision, minvoltage double precision, maxvoltage double precision, maxcurrent double precision, maxpower double precision) 
    LANGUAGE 'plpgsql'
    COST 100
    VOLATILE PARALLEL UNSAFE
    ROWS 1000

AS $BODY$
begin
	return query (
select t.source, max(t.dt) as dt, t.isday,SUM(t.Consumption) as Consumption, 
	min(t.MinVoltage) as MinVoltage, max(t.MaxVoltage) as MaxVoltage,
	Max(t.MaxCurrent) as MaxCurrent, max(t.MaxPower) as MaxPower
from (
select e.source, e.indicator, e.dt, e.isday,0 as Consumption, 
	min(e.val) as MinVoltage, max(e.val) as MaxVoltage, 
	0 as MaxCurrent, 0 as MaxPower
from "EnergyView" e
where e.indicator = 'Voltage' and (e.dt>="DateFrom" and e.dt<="DateTo")
group by e.source, e.indicator, e.dt, e.isday
UNION ALL 
select e.source, e.indicator, e.dt, e.isday,0 as Consumption,999999  as MinVoltage, 0 as MaxVoltage, 
	max(e.val) as MaxCurrent, 0 as MaxPower
from "EnergyView" e
where e.indicator = 'Current' and (e.dt>="DateFrom" and e.dt<="DateTo")
group by e.source, e.indicator, e.dt, e.isday
UNION ALL 
select e.source, e.indicator, e.dt, e.isday,0 as Consumption,999999  as MinVoltage, 0 as MaxVoltage, 0 as MaxCurrent, 
	 max(e.val) as MaxPower
from "EnergyView" e
where e.indicator = 'Power' and (e.dt>="DateFrom" and e.dt<="DateTo")
group by e.source, e.indicator, e.dt, e.isday

UNION ALL 
select e1.source, e1.indicator, e1.dt, e1.isday,e1.Consumption, 
	999999 as MinVoltage, 0 as MaxVoltage, 0 as MaxCurrent, 0 as MaxPower
from consumedEnergy("DateFrom","DateTo") e1
) t
group by t.source, t.isday
);		
end;
$BODY$;

ALTER FUNCTION public.energystat(date, date)
    OWNER TO iobroker;
