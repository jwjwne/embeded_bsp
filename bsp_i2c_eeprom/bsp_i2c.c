#include "bsp_i2c.h"

/**
  * @brief   写一个字节到I2C EEPROM中
  * @param   
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写地址 
  * @retval  无
  */
uint32_t I2C_EE_ByteWrite(uint8_t* pBuffer, uint8_t WriteAddr)
{
	HAL_StatusTypeDef status = HAL_OK;

	status = HAL_I2C_Mem_Write(&hi2c1, EEPROM_ADDRESS, (uint16_t)WriteAddr, I2C_MEMADD_SIZE_8BIT, pBuffer, 1, 100); 

	/* Check the communication status */
	if(status != HAL_OK)
	{
	/* Execute user timeout callback */
	//I2Cx_Error(Addr);
	}
	while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
	{
		
	}

	/* Check if the EEPROM is ready for a new operation */
	while (HAL_I2C_IsDeviceReady(&hi2c1, EEPROM_ADDRESS, EEPROM_MAX_TRIALS, I2Cx_TIMEOUT_MAX) == HAL_TIMEOUT);

	/* Wait for the end of the transfer */
	while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
	{
		
	}
	return status;
}

/**
  * @brief   在EEPROM的一个写循环中可以写多个字节，但一次写入的字节数
  *          不能超过EEPROM页的大小，AT24C02每页有8个字节
  * @param   
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写地址
  *     @arg NumByteToWrite:写的字节数
  * @retval  无
  */
uint32_t I2C_EE_PageWrite(uint8_t* pBuffer, uint8_t WriteAddr, uint8_t NumByteToWrite)
{
	HAL_StatusTypeDef status = HAL_OK;
	/* Write EEPROM_PAGESIZE */
	//I2C_Handle I2C配置信息   EEPROM_ADDRESS 从机地址   WriteAddr 开始写的地址   I2C_MEMADD_SIZE_8BIT 数据长度
	//pBuffer 数据   NumByteToWrite 要发送的数据量  100 超时时间
	status=HAL_I2C_Mem_Write(&hi2c1, EEPROM_ADDRESS,WriteAddr, I2C_MEMADD_SIZE_8BIT, (uint8_t*)(pBuffer),NumByteToWrite, 100);

	while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
	{
		
	}

	/* Check if the EEPROM is ready for a new operation *///检查EEPROM是否已经准备好进行新的操作
	while (HAL_I2C_IsDeviceReady(&hi2c1, EEPROM_ADDRESS, EEPROM_MAX_TRIALS, I2Cx_TIMEOUT_MAX) == HAL_TIMEOUT);

	/* Wait for the end of the transfer */
	while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
	{
		
	}
	return status;
}

/**
  * @brief   从EEPROM里面读取一块数据 
  * @param   
  *		@arg pBuffer:存放从EEPROM读取的数据的缓冲区指针
  *		@arg WriteAddr:接收数据的EEPROM的地址
  *     @arg NumByteToWrite:要从EEPROM读取的字节数
  * @retval  无
  */
uint32_t I2C_EE_BufferRead(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{
	HAL_StatusTypeDef status = HAL_OK;
	
	status=HAL_I2C_Mem_Read(&hi2c1,EEPROM_ADDRESS,ReadAddr, I2C_MEMADD_SIZE_8BIT, (uint8_t *)pBuffer, NumByteToRead,1000);

	return status;
}

/**
  * @brief   将缓冲区中的数据写到I2C EEPROM中
  * @param   
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写地址
  *     @arg NumByteToWrite:写的字节数
  * @retval  无
  */
void I2C_EE_BufferWrite(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)//数组 写地址 写多少个字
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;

  Addr = WriteAddr % EEPROM_PAGESIZE;//写地址%8   八个字节被占用了多少个字节
  count = EEPROM_PAGESIZE - Addr;//8-ADDr         要写多少个字节
  NumOfPage =  NumByteToWrite / EEPROM_PAGESIZE;//写多少个字/8     写多少行
  NumOfSingle = NumByteToWrite % EEPROM_PAGESIZE;//写多少个字%8     写多少行之后还剩下多少
 
  /* If WriteAddr is I2C_PageSize aligned  */
  if(Addr == 0)//判断写地址是不是eeprom页(8)的对齐
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfPage == 0) //判断如果写不满一行  就写NumOfSingle个字
    {
      I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
    }
    /* If NumByteToWrite > I2C_PageSize */
    else  
    {
      while(NumOfPage--)//重写多少行（NumOfPage）开始递减  先使用再--
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, EEPROM_PAGESIZE); 
        WriteAddr +=  EEPROM_PAGESIZE;//开始写的基地址加EEPROM_PAGESIZE
        pBuffer += EEPROM_PAGESIZE;//写的数据加EEPROM_PAGESIZE
      }

      if(NumOfSingle!=0)//把剩下的数据写如
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
  /* If WriteAddr is not I2C_PageSize aligned  */   //不是eeprom页(8)的对齐
  else 
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfPage== 0)   //判断如果写不满一行  就写NumOfSingle个字
    {
      I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
    }
    /* If NumByteToWrite > I2C_PageSize */
    else  
    {
      NumByteToWrite -= count;//要写多少个字-=要写多少个字节
      NumOfPage =  NumByteToWrite / EEPROM_PAGESIZE;//写多少行   给NumOfPage重新赋值 NumByteToWrite（要写多少个字-=要写多少个字节）/EEPROM_PAGESIZE
      NumOfSingle = NumByteToWrite % EEPROM_PAGESIZE;	//写多少行之后还剩下多少   给NumOfSingle重新赋值 NumByteToWrite% EEPROM_PAGESIZE
      
      if(count != 0)//如果那一行还需要写count个字节
      {  
        I2C_EE_PageWrite(pBuffer, WriteAddr, count);
        WriteAddr += count;
        pBuffer += count;
      } 
      
      while(NumOfPage--)//写入NumOfPage行
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, EEPROM_PAGESIZE);
        WriteAddr +=  EEPROM_PAGESIZE;
        pBuffer += EEPROM_PAGESIZE;  
      }
      if(NumOfSingle != 0)//再写入NumOfSingle个字节
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle); 
      }
    }
  }  
}
