# Garbage Collector, a source control system that just does what its told.

intended for projects of _small_ teams, the definition of which is as follows:

    N-way conflicts should not occur on a daily basis. (N > 2)

## USAGE

$ gc init  
    initializes a `.gc` directory, where all gc-related data will be stored.

$ gc save \[ -m "message" ]  
    saves the current state with an optional message.

$ gc push  
    sends the last saved state state to the remote.

$ gc pull  
    requests the latest state from the remote.

$ gc remote set 'URL'  
    sets the remote to the provided value.

$ gc remote get  
    prints the current remote to the stdout.

$ gc config 'NAME' 'EMAIL'  
    sets the current user's name and email.
