# nvcs, iNvisible Version Control System

intended for projects of _small_ teams, the definition of which is as follows:

    Per any time pediod, e.g. a day, no file should be modified in a conflicting way by more than two people.

consider the following project:
```
src/
src/foo
src/bar
src/baz
```

the project is developed by 2 people: Alice and Bob

- Day 1:
    - Alice makes changes to `foo` and `bar`.
    - Bob makes changes to `bar` and `baz`.
        - (Note: Bob's changes to `bar` are conflicting with Alice's changes)
    - Alice "pushes" her work to the remote server
    - Bob "pushes" his work to the remote server
    - The VCS detects a conflict, then sends emails to the people who were responsible, then saves both their works.

- Day 2:
    - Alice "pulls" the remote project, her local project now is:
    ```
    src/
    src/foo
    src/bar
    src/bar-[Bob]
    src/baz
    ```
    - Bob "pulls" the remote project, his local project now is:
    ```
    src/
    src/foo
    src/bar
    src/bar-[Alice]
    src/baz
    ```
    - Alice and Bob continue using their files, ignoring the `-[NAME]` conflicts in the meantime.

- Day 3:
    - Alice and Bob talk and decide to resolve the conflict.
    - They decide that Bob should resolve the conflict.
    - Alice waits for Bob or does other work that is non-dependent on the files in the conflict.
    - After Bob is done, he pushes his work to the remote and notifies Alice. His local project is:
    ```
    src/
    src/foo
    src/bar
    src/baz
    ```
    - Alice pulls the project.
