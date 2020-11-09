/*
 * Status-Codes des Publish-Subscriber-Systems.
 * Die Ausgabe der passenden Status-Meldungen kann über
 * das Feld PUB_SUB_RET_CODE erfolgen.
 * Bsp.: printf ("%s\n", PUB_SUB_RET_CODE[*res_code])
 * sofern in short* res_code die Antwort auf die Anfrage
 * des RPC enthalten ist. 
 */

#ifndef RETURN_CODES_H
#define	RETURN_CODES_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

#define OK                          0
#define CANNOT_REGISTER             1
#define CLIENT_ALREADY_REGISTERED   2
#define CANNOT_UNREGISTER           3
#define CANNOT_SET_TOPIC            4
#define UNKNOWN_ERROR               5
#define WRONG_PASSWORD              6
#define NO_SESSION_FOUND            7
#define NO_ACCOUNT_FOUND            8
#define CORRUPTED_TRANSFER          9

static char* PUB_SUB_RET_CODE[] = {
        "Ok.",                         /* OK                        */
        "Cannot register client.",     /* CANNOT_REGISTER           */
        "Client already registered.",  /* CLIENT_ALREADY_REGISTERED */
        "Cannot unregister client.",   /* CANNOT_UNREGISTER         */
        "Cannot set topic.",           /* CANNOT_SET_TOPIC          */
        "Unknown error.",              /* UNKNOWN_ERROR             */
        "Wrong Password.",           /* NO_SESSION_FOUND          */
        "No session found.",           /* WRONG_PASSWORD            */
        "No account with this username.", /*NO_ACOUNT_FOUND*/
        "The transfer between client and server was corrupted." /* CORRPTED_TRANSFER */
};

typedef short RET_CODE;

#endif /* RETURN_CODES_H */

