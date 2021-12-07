-- FUNCTION: public.consumedenergy(date, date)

-- DROP FUNCTION IF EXISTS public.consumedenergy(date, date);

CREATE OR REPLACE FUNCTION public.consumedenergy(
	"DateFrom" date,
	"DateTo" date)
    RETURNS TABLE(sect text, d_cons real, n_cons real) 
    LANGUAGE 'plpgsql'
    COST 100
    VOLATILE PARALLEL UNSAFE
    ROWS 1000

AS $BODY$
begin
return query (
select t2.sect, Max(day_cons) as d_cons, max(night_cons) as n_cons
from (
select "section" as sect, day_night,
case when day_night=true then max(e) - min(e) else 0 end as day_cons,
case when day_night=false then max(e) - min(e) else 0 end as night_cons
from (
select section,day_night, energy_a+energy_b+energy_c as e
  from "PowerValues"
  where ts>="DateFrom" AND ts<"DateTo" and (energy_a!=0 and energy_a is not NULL) and
  (energy_b!=0 and energy_b is not NULL) AND
  (energy_c!=0 and energy_c is not NULL)
) t1
group by "section", day_night
)t2 
group by t2.sect
	
);

end;
$BODY$;

ALTER FUNCTION public.consumedenergy(date, date)
    OWNER TO iobroker;
