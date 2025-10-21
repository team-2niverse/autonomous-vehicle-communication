#ifndef BSW_ENCODER_ENCODER_H_
#define BSW_ENCODER_ENCODER_H_

// Function Prototypes
unsigned int Encoder_Get_Rpm_Left(void);
unsigned int Encoder_Get_Rpm_Right(void);

void Encoder_Init(void);

void Encoder_pollCanSend(void);

#endif /* BSW_ENCODER_ENCODER_H_ */
