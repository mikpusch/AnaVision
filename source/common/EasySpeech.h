// EasySpeech.h v.20070525 © aljodav
// from the article: http://www.codeproject.com/useritems/MFCSpeaksEasily.asp
// uses Microsoft Speech Object Library 5.0+
// http://msdn.microsoft.com/library/default.asp?url=/library/en-us/SAPI51sr/Whitepapers/WP_XML_TTS_Tutorial.asp
// http://msdn.microsoft.com/library/default.asp?url=/library/en-us/SAPI51sr/Whitepapers/WP_Setup_Whitepaper_51.asp
// http://www.microsoft.com/downloads/details.aspx?FamilyID=5e86ec97-40a7-453f-b0ee-6583171b4530&DisplayLang=en
// SDK 5.1: http://download.microsoft.com/download/speechSDK/SDK/5.1/WXP/EN-US/SpeechSDK51.exe

//long CEasySpeech::Speak(const LPCTSTR psz, long Flag = 0, long Flag2 = -1)
//    long Flag2 : defaults to -1 if not specified.
//                : -1 => psz and Flag are considered as described below.
//                :  0 => pauses the current async speech, if any; other arguments are ignored.
//                :  1 => resumes the paused async speech, if any; other arguments are ignored.
//                : any other value is treated as invalid flag Flag2.
//   LPCTSTR psz : depends on Flag.
//     long Flag : defaults to 0 if not specified.
//               : 0 =>  sync mode; psz points to a text to be spoken. 
//               : 1 => async mode; psz points to a text to be spoken. 
//               : 2 => purge current async speech (if any), and proceed as in f==0
//               : 3 => purge current async speech (if any), and proceed as in f==1
//               : 4 =>  sync mode; psz points to a filename containing the text to be spoken. 
//               : 5 => async mode; psz points to a filename containing the text to be spoken. 
//               : 6 => purge current async speech (if any), and proceed as in f==4
//               : 7 => purge current async speech (if any), and proceed as in f==5
//               : any other value is treated as Flag equals to 0.
// 
//    when in  sync mode, the application stops running until the speech is done.
//    when in async mode, the application keeps running without waiting anything.
//
//    returned value :  1 => method succeeded.
//                   :  0 => method failed; COM object returned an error.
//                   : -1 => method failed; psz is not OK, 
//                           or Microsoft Speech Object Library is not registered.
//
//
//long CEasySpeech::Speak(long Flag2 = -1)
//    long Flag2 : defaults to -1 if not specified.
//               : -1 => purges current speech
//               :  0 => pauses the speech
//               :  1 => resumes the speech
//               :  any other value is treated as invalid Flag2.
//
//    returned value :  1 => method succeeded.
//                   :  0 => method failed; COM object returned an error.
//                   : -1 => method failed; psz is not OK, 
//                           or Microsoft Speech Object Library is not registered.
//                   : -2 => method failed; invalid Flag2.
//

#ifndef _MFC_Speaks_Easily_aljodav_
#define _MFC_Speaks_Easily_aljodav_
/*
#pragma message("°"                                                                                    )
#pragma message("°   using class CEasySpeech;"                                                         )
#pragma message("°   you can use it in any free and non commercial product with the following citation")
#pragma message("°   (maybe in your 'About dialog box'):"                                              )
#pragma message(" "                                                                                    )
#pragma message("    CEasySpeech class code © aljodav, from the article :"                             )
#pragma message("    http://www.codeproject.com/useritems/MFCSpeaksEasily.asp"                         )
#pragma message(" "                                                                                    )
#pragma message("°"                                                                                    )
*/
class CEasySpeech { // © aljodav
public:
	// *** properties ***
	__declspec(property(get=GetCurrentVoiceAttr                          )) const CString  CurrentVoiceAttr ;
	__declspec(property(get=GetVoiceAttr                                 )) const CString  VoiceAttr[]      ;
	__declspec(property(                          put=PutCurrentVoice    )) long           CurrentVoice     ;
	__declspec(property(                          put=PutSyncSpeech      )) const LPCTSTR  SyncSpeech       ;
	__declspec(property(                          put=PutSyncFileSpeech  )) const LPCTSTR  SyncFileSpeech   ;
	__declspec(property(                          put=PutAsyncSpeech     )) const LPCTSTR  AsyncSpeech      ;
	__declspec(property(                          put=PutAsyncFileSpeech )) const LPCTSTR  AsyncFileSpeech  ;
	__declspec(property(get=GetRate             , put=PutRate            )) long           Rate             ;
	__declspec(property(get=GetVolume           , put=PutVolume          )) long           Volume           ;
	__declspec(property(get=GetVoices                                    )) long           Voices           ;
	__declspec(property(                          put=PutPause           )) BOOL           Pause            ;
	__declspec(property(get=GetIsSpeaking                                )) BOOL           IsSpeaking       ;
	__declspec(property(get=GetStreamName       , put=PutStreamName      )) const CString& StreamName       ;
	__declspec(property(get=GetSyncStreamSpeech , put=PutSyncStreamSpeech)) const LPCTSTR  SyncStreamSpeech ;
	__declspec(property(get=GetAsyncStreamSpeech                         )) const LPCTSTR  AsyncStreamSpeech;
	__declspec(property(get=GetHandle                                    )) HANDLE         Handle           ;
	__declspec(property(get=GetLastBookmarkID                            )) long           LastBookmarkID   ;
	__declspec(property(get=GetLastBookmark                              )) const CString  LastBookmark     ;
	__declspec(property(get=GetVisemeID                                  )) long           VisemeID         ;
	// *** ancestor methods ***
    long Speak(const LPCTSTR p, long f = 0, long f2 = -1) {
        switch (f2) {
        case -1: return m_sv.Speak(p, f); // speak (maybe purging first)
        default: return Speak(f2);        // pauses or resumes the speech; ignore other arguments
        }
    }
    long Speak(long f2 = -1) {
        switch (f2) {
        case -1:        return m_sv.Speak  (_T(""), 3); // purges current async speech
        case 0: case 1: return m_sv.DMethod(0x0e + f2); // pauses or resumes the speech
        default:        return -2;                      // invalid flag f2
        }
    }
	// *** accessors ***
	const CString  GetCurrentVoiceAttr (                ) { return m_sv.GetVoice(-1);                                  }
	const CString  GetVoiceAttr        (long l          ) { return m_sv.GetVoice(l);                                   }
	void           PutCurrentVoice     (long l          ) { m_sv.PutVoice(l);                                          }
	void           PutSyncSpeech       (const LPCTSTR p ) { if (p) m_sv.Speak(p, 0); else m_sv.Speak(_T(""), 3);       }
	void           PutSyncFileSpeech   (const LPCTSTR p ) { if (p) m_sv.Speak(p, 4); else m_sv.Speak(_T(""), 3);       }
	void           PutAsyncSpeech      (const LPCTSTR p ) { if (p) m_sv.Speak(p, 1); else m_sv.Speak(_T(""), 3);       }
	void           PutAsyncFileSpeech  (const LPCTSTR p ) { if (p) m_sv.Speak(p, 5); else m_sv.Speak(_T(""), 3);       }
	long           GetRate             (                ) { return m_sv.DProperty(0x05);                               }
	void           PutRate             (long l          ) { m_sv.DProperty(0x05, l);                                   }
	long           GetVolume           (                ) { return m_sv.DProperty(0x06);                               }
	void           PutVolume           (long l          ) { m_sv.DProperty(0x06, (l < 0) ? 0 : ((l > 100) ? 100 : l)); }
    long           GetVoices           (                ) { return m_sv.Voices();                                      }
	void           PutPause            (BOOL b          ) { if (b) m_sv.DMethod(0x0e); else m_sv.DMethod(0x0f);        }
	BOOL           GetIsSpeaking       (                ) { return (m_sv.Status(0x04) == 2);                           }
	const CString& GetStreamName       (                ) { return m_sv.StreamName();                                  }
	void           PutStreamName       (const CString& s) { if (!s.IsEmpty()) m_sv.StreamName(s);                      }
	const LPCTSTR  GetSyncStreamSpeech (                ) { return m_sv.StreamSpeech();                                }
	void           PutSyncStreamSpeech (const LPCTSTR p ) { m_sv.StreamSpeech(p);                                      }
	const LPCTSTR  GetAsyncStreamSpeech(                ) { return m_sv.StreamSpeech(1);                               }
	HANDLE         GetHandle           (                ) { return m_sv.Handle();                                      }
	long           GetLastBookmarkID   (                ) { return m_sv.Status(0x0a);                                  }
	const CString  GetLastBookmark     (                ) { return m_sv.LastBookmark();                                }
	long           GetVisemeID         (                ) { return m_sv.Status(0x0c, TRUE);                            }
private:
	class CSpeechVoice : public COleDispatchDriver {
	public:
		CSpeechVoice() : c_bcom(TRUE), m_strStreamName(_T("Sample.wav")) { if (c_bcom) ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED); CreateDispatch(_T("SAPI.SpVoice")); }
		~CSpeechVoice()                                                  { ReleaseDispatch(); if (c_bcom) ::CoUninitialize(); }
		long Speak(const LPCTSTR p, long f) {
			if     (!p || !m_lpDispatch) return long(-1);                                                
			switch (f) { default: f = 0; case 1: case 2: case 3: case 4: case 5: case 6: case 7: ; }
			static BYTE pm[] = VTS_BSTR VTS_I4; long r = 0;
			try    {InvokeHelper(0x0c, DISPATCH_METHOD, VT_I4, (void*)&r, pm, p, f);}
			catch  (COleDispatchException* e) { e->Delete(); }
			catch  (COleException*         e) { e->Delete(); }
			return r;
		}
		long DMethod(DISPID d) {
			if     (!m_lpDispatch) return long(-1); long r = 1;                                          
			try    {InvokeHelper(d, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);}
			catch  (COleDispatchException* e) { e->Delete(); r = 0; }
			catch  (COleException*         e) { e->Delete(); r = 0; }
			return r;
		}
		long DProperty(DISPID d) { long r = LONG_MIN;                                                    
		if         (m_lpDispatch) {
			try    {InvokeHelper(d, DISPATCH_PROPERTYGET, VT_I4, (void*)&r, NULL);}
			catch  (COleDispatchException* e) { e->Delete(); r = LONG_MIN; }
			catch  (COleException*         e) { e->Delete(); r = LONG_MIN; }
		} return   r;
		}
		void DProperty(DISPID d, long l) {
			if     (!m_lpDispatch) return; static BYTE pm[] = VTS_I4;                                    
			try    {InvokeHelper(d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, pm, l);}
			catch  (COleDispatchException* e) { e->Delete(); }
			catch  (COleException*         e) { e->Delete(); }
		}
		long Voices() {
			if       (!m_lpDispatch) return 0; LPDISPATCH pd = NULL; long r = 0;
			try      {InvokeHelper(0x11, DISPATCH_METHOD, VT_DISPATCH, (void*)&pd, NULL);}               
			catch    (COleDispatchException* e) { e->Delete(); }
			catch    (COleException*         e) { e->Delete(); }
			if       (pd) { COleDispatchDriver odd(pd);                                                  
			try      {odd.InvokeHelper(0x01, DISPATCH_PROPERTYGET, VT_I4, (void*)&r, NULL);}             
			catch    (COleDispatchException* e) { e->Delete(); }
			catch    (COleException*         e) { e->Delete(); }
			} return r;
		}
		const CString GetVoice(long l) {
			if (!m_lpDispatch || (l >= Voices())) return _T(""); LPDISPATCH pd = NULL;
			if (l < 0) {                                                                                 
				try    {InvokeHelper(0x02, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pd, NULL);}        
				catch  (COleDispatchException* e) { e->Delete(); }
				catch  (COleException*         e) { e->Delete(); }
			}
			else   {
				try    {InvokeHelper(0x11, DISPATCH_METHOD, VT_DISPATCH, (void*)&pd, NULL);}             
			catch  (COleDispatchException* e) { e->Delete(); }
			catch  (COleException*         e) { e->Delete(); }
			if     (pd) { COleDispatchDriver odd(pd);                                                    
			static BYTE pm[] = VTS_I4; pd = NULL;
			try    {odd.InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&pd, pm, l);}             
			catch  (COleDispatchException* e) { e->Delete(); }
			catch  (COleException*         e) { e->Delete(); }
			}
			}
			if (!pd) return _T("");
			LPCTSTR ppsz[] = { _T("Description"), _T("Age"), _T("Gender"), _T("Language"), _T("Name"), _T("Vendor"), };
			COleDispatchDriver odd(pd); CString str(_T(' '), 512), sx(_T(' '), 64);                      
			sx.Format(_T("[%s]%s"), ppsz[0], Info(&odd         ));   str  = sx;
			for (int i = 1; i < (sizeof(ppsz) / sizeof(ppsz[0])); ++i)
			{ sx.Format(_T("[%s]%s"), ppsz[i], Info(&odd, ppsz[i])); str += sx; }
			return (str.FreeExtra(), str);
		}
		BOOL PutVoice(long l) {
			if     (!m_lpDispatch || (l < 0) || (l >= Voices())) return FALSE; LPDISPATCH pd = NULL;
			try    {InvokeHelper(0x11, DISPATCH_METHOD, VT_DISPATCH, (void*)&pd, NULL);}                 
			catch  (COleDispatchException* e) { e->Delete(); }
			catch  (COleException*         e) { e->Delete(); }
			if     (pd) { COleDispatchDriver odd(pd);                                                    
			static BYTE pm[] = VTS_I4; pd = NULL;
			try    {odd.InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&pd, pm, l);}             
			catch  (COleDispatchException* e) { e->Delete(); }
			catch  (COleException*         e) { e->Delete(); }
			}
			if       (pd) { static BYTE pm[] = VTS_DISPATCH;                                             
			try      {InvokeHelper(0x02, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, pm, pd);}              
			catch    (COleDispatchException* e) { e->Delete(); }
			catch    (COleException*         e) { e->Delete(); }
			} return pd ? (pd->Release(), TRUE) : FALSE;
		}
		const CString& StreamName(               ) { return m_strStreamName; }
		void           StreamName(const CString& s) { if (!s.IsEmpty()) m_strStreamName = s;
		if (m_strStreamName.Right(4).MakeLower() != _T(".wav")) m_strStreamName += _T(".wav");
		}
		const LPCTSTR StreamSpeech(long f = 0) { // f==1 => async, f!=1 => sync
			if     (!m_lpDispatch) return _T(""); COleDispatchDriver odd; odd.CreateDispatch(_T("SAPI.SpFileStream"));
			if     (!odd.m_lpDispatch) return _T(""); static BYTE pm2[] = VTS_BSTR; long r = 0;
			try    {odd.InvokeHelper(0x64, DISPATCH_METHOD, VT_EMPTY, NULL, pm2, m_strStreamName);}      
			catch  (COleDispatchException* e) { e->Delete(); r = -1; }
			catch  (COleException*         e) { e->Delete(); r = -1; }
			if     (r == -1) return _T(""); static BYTE pm[] = VTS_DISPATCH VTS_I4;
			switch (f) { default: f = 0; case 1: ; }
			try    {InvokeHelper(0x0d, DISPATCH_METHOD, VT_I4, (void*)&r, pm, odd.m_lpDispatch, f);}     
			catch  (COleDispatchException* e) { e->Delete(); r = -1; }
			catch  (COleException*         e) { e->Delete(); r = -1; }
			return (r == 1) ? m_strStreamName : _T("");
		}
		void StreamSpeech(const LPCTSTR p) {
			if     (!m_lpDispatch) return; LPDISPATCH pd = NULL;
			try    {InvokeHelper(0x04, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pd, NULL);}            
			catch  (COleDispatchException* e) { e->Delete(); }
			catch  (COleException*         e) { e->Delete(); }
			if     (!pd) return; COleDispatchDriver odd; odd.CreateDispatch(_T("SAPI.SpFileStream"));    
			if     (!odd.m_lpDispatch) return; static BYTE pm2[] = VTS_BSTR VTS_I4; long r = 0;
			try    {odd.InvokeHelper(0x64, DISPATCH_METHOD, VT_EMPTY, NULL, pm2, m_strStreamName, 3);}   
			catch  (COleDispatchException* e) { e->Delete(); r = -1; }
			catch  (COleException*         e) { e->Delete(); r = -1; }
			if     (r == -1) return; static BYTE pm[] = VTS_DISPATCH ;
			try    {InvokeHelper(0x04, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, pm, odd.m_lpDispatch);}  
			catch  (COleDispatchException* e) { e->Delete(); r = -1; }
			catch  (COleException*         e) { e->Delete(); r = -1; }
			if     (r == -1) return; r = Speak(p, 0);                                                    
			try    {odd.InvokeHelper(0x65, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);}                      
			catch  (COleDispatchException* e) { e->Delete(); }
			catch  (COleException*         e) { e->Delete(); }
			try    {InvokeHelper(0x04, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, pm, pd);}                
			catch  (COleDispatchException* e) { e->Delete(); }
			catch  (COleException*         e) { e->Delete(); }
			pd->Release();
		}
		HANDLE Handle() {
			if     (!m_lpDispatch) return 0; long r = 0;
			try    {InvokeHelper(0x14, DISPATCH_METHOD, VT_I4, (void*)&r, NULL);}
			catch  (COleDispatchException* e) { e->Delete(); }
			catch  (COleException*         e) { e->Delete(); }
			return HANDLE(LONGLONG(r));
		}
		long Status(DISPID d, BOOL bshort = FALSE) { 
			if       (!m_lpDispatch) return long(-1); long r = -1; short s = -1; LPDISPATCH pd = NULL;
			try      {InvokeHelper(0x01, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pd, NULL);}          
			catch    (COleDispatchException* e) { e->Delete(); }
			catch    (COleException*         e) { e->Delete(); }
			if       (pd) { COleDispatchDriver odd(pd);                                                  
			try      {odd.InvokeHelper(d, DISPATCH_PROPERTYGET, bshort ? VARTYPE(VT_I2) : VARTYPE(VT_I4), bshort ? (void*)&s : (void*)&r, NULL);}
			catch    (COleDispatchException* e) { e->Delete(); }
			catch    (COleException*         e) { e->Delete(); }
			} return bshort ? long(s) : r;
		}
		const CString LastBookmark() {
			if       (!m_lpDispatch) return _T(""); CString str; LPDISPATCH pd = NULL;
			try      {InvokeHelper(0x01, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pd, NULL);}          
			catch    (COleDispatchException* e) { e->Delete(); }
			catch    (COleException*         e) { e->Delete(); }
			if       (pd) { COleDispatchDriver odd(pd);
			try      {odd.InvokeHelper(0x09, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&str, NULL);}         
			catch    (COleDispatchException* e) { e->Delete(); }
			catch    (COleException*         e) { e->Delete(); }
			} return str;
		}
	private:
		const BOOL c_bcom;
		CString m_strStreamName;
		const CString Info(COleDispatchDriver* p, LPCTSTR psz = NULL) { CString str;                     
		if (p) { static BYTE pm[] = VTS_BSTR;
		try {
			if (psz) p->InvokeHelper(0x06, DISPATCH_METHOD, VT_BSTR, (void*)&str, pm , psz);             
			else     p->InvokeHelper(0x04, DISPATCH_METHOD, VT_BSTR, (void*)&str, NULL);                 
		}
		catch    (COleDispatchException* e) { e->Delete(); }
		catch    (COleException*         e) { e->Delete(); }
		} return str.IsEmpty() ? _T("?") : str;
		}
	} m_sv;
};

#endif //_MFC_Speaks_Easily_aljodav_
