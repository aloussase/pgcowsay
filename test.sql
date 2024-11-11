drop extension if exists pgcowsay;
create extension pgcowsay;

select pgcowsay.say('hello world');
