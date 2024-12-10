#pragma once

#define LOG(msg) UE_LOG(LogTemp, Warning, TEXT(msg));

#define LOG_ERROR(msg) UE_LOG(LogTemp, Error, TEXT(msg));

#define RETURN_LOG(cond, msg) \
				if (cond) \
				{ \
					UE_LOG(LogTemp, Warning, TEXT(msg)); \
					return; \
				}