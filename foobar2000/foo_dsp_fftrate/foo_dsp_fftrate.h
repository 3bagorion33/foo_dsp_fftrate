#ifndef _FOO_DSP_FFT_RATE_H_
#define _FOO_DSP_FFT_RATE_H_

#include <foobar2000/helpers/foobar2000+atl.h>
#include "swin.h"
#include "convert.h"
#include "resource.h"


struct t_foo_dsp_fftrate_desc
{
	const TCHAR	*name;
	t_int32		val;
};


class foo_dsp_fftrate_params
{
public:
	foo_dsp_fftrate_params( void ):
		m_trans( CONV_TRANSFORM_FFT ),
		m_wind( CONV_WINDOW_VORBIS ),
		m_rate( 0 ),
		m_chann( 0 )
	{
	}

	// The GUID that identifies this DSP and its configuration.
	static const GUID &g_get_guid( void );

	// Read data from a preset.
	bool set_data( const dsp_preset &p_data );
	bool get_data( dsp_preset &p_data );

	void set_trans( t_int32 p_trans )
	{
		m_trans = p_trans;
	}

	void set_wind( t_int32 p_wind )
	{
		m_wind = p_wind;
	}

	void set_rate( t_int32 p_rate )
	{
		m_rate = p_rate;
	}

	void set_chann( t_int32 p_chann )
	{
		m_chann = p_chann;
	}

	t_int32 trans( void ) const
	{
		return m_trans;
	}

	t_int32 wind( void ) const
	{
		return m_wind;
	}

	t_int32 rate( void ) const
	{
		return m_rate;
	}

	t_int32 chann( void ) const
	{
		return m_chann;
	}

	void set_rateList(const pfc::string8& rateList) { m_rateList = rateList; }
	pfc::string8 get_rateList() const { return m_rateList; }

private:
	pfc::string8 m_rateList;

	static const t_size	m_paramCount = 4;

	union
	{
		t_int32	m_config[1];

		struct 
		{
			t_int32	m_trans;
			t_int32	m_wind;
			t_int32	m_rate;
			t_int32	m_chann;
		};
	};
};


class foo_dsp_fftrate_dialog: public CDialogImpl <foo_dsp_fftrate_dialog>
{
public:
	enum
	{
		IDD = IDD_CONFIG
	};
 
	BEGIN_MSG_MAP_EX( foo_dsp_fftrate_dialog )
		MSG_WM_INITDIALOG( OnInitDialog )
		MSG_WM_COMMAND( OnCommand )
	END_MSG_MAP()

	foo_dsp_fftrate_dialog( foo_dsp_fftrate_params &p_params );


protected:
	BOOL OnInitDialog( CWindow wndFocus, LPARAM lInitParam );
	void OnCommand( UINT uNotifyCode, int nID, CWindow wndCtl );

private:
	foo_dsp_fftrate_params &m_params;

	void combo_upload( int id, const t_foo_dsp_fftrate_desc *desc, t_int32 curr );
	t_int32 combo_download( int id, const t_foo_dsp_fftrate_desc *desc );

public:
	pfc::string8 getRateList() const { return m_rateList; }

private:
	pfc::string8 m_rateList;
};


class foo_dsp_fftrate: public dsp_impl_base_t<dsp_v2>
{
public:
	foo_dsp_fftrate( const foo_dsp_fftrate_params &params );
	~foo_dsp_fftrate( void );

	void flush( void );
	double get_latency( void );
	bool need_track_change_mark( void );

protected:
	virtual void on_endoftrack( abort_callback &p_abort );
	virtual void on_endofplayback( abort_callback &p_abort );
	bool on_chunk( audio_chunk *chunk, abort_callback &p_abort );

private:
	static const t_size m_bufferSize = 32768;

	HCONVERT	m_convert;
	BYTE		m_buffer[m_bufferSize];

	int			m_transRate;
	int			m_transChann;

	int			m_outRate;
	int			m_outChann;

	int			m_trans;
	int			m_wind;

	int			m_rate;
	int			m_chann;
	int			m_map;

	void close( void );
	void open( int p_rate, int p_chann, int p_map );

public:
	void setRateList(const pfc::string8& rateList) { m_rateList = rateList; }

private:
	pfc::string8 m_rateList;
};

#endif //_FOO_DSP_FFT_RATE_H_
