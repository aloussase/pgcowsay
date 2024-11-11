-- pgweb--0.0.1.sql

create schema pgcowsay;

create or replace function pgcowsay.say(TEXT)
returns text as 'pgcowsay', 'pgcowsay_say'
language c strict;

grant execute on function pgcowsay.say(TEXT) to public;
