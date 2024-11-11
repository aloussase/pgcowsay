# pgcowsay

This is a postgres extension I made for educational purposes.

```sql
select pgcowsay.say('Hello!');
```

Output:

```
             say
------------------------------
  _______                    +
 < Hello >                   +
  -------                    +
         \   ^__^            +
          \  (oo)\_______    +
             (__)\       )\/\+
                 ||----w |   +
                 ||     ||   +

(1 row)
```

## Relevant links

- EDB article on building a web server in a postgres extension: https://www.enterprisedb.com/blog/exploring-postgress-arena-allocator-writing-http-server-scratch.
- Postgres documentation on C API functions: https://www.postgresql.org/docs/6.5/xfunc14663.htm

## License

MIT
